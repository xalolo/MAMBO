#include <vif.hpp>
#include "egg-utils.hpp"

using namespace vif;

void print_help();

int vif_main(int argc, char* argv[]) {
    uint_t id = npos;
    std::string component, seds, out;
    bool ascii = false;
    read_args(argc, argv, arg_list(seds, out, id, component, ascii));

    if (seds.empty() || id == npos) {
        print_help();
        return 0;

    }

    if (out.empty()) {
        std::string cmp_str;

        if (!component.empty()) {
            cmp_str = component+"-";
        }

        out = file::remove_extension(seds)+"-"+cmp_str+to_string(id)+(ascii ? ".cat" : ".fits");
    } else {
        file::mkdir(file::get_directory(out));
    }

    vec1u ids;
    uint_t elem_size;
    fits::read_table(file::remove_extension(seds)+"-lookup.fits",
        "id", ids, "elem_size", elem_size
    );

    if (elem_size != sizeof(float)) {
        error("this spectrum file was created using another incompatible computer");
        note("the size of a single float number is different than expected");
        note("the file cannot be read");
        return 1;
    }

    uint_t nid = where_first(ids == id);

    if (nid == npos) {
        error("there is no galaxy with ID=", id, " in this catalog");
        return 1;
    }

    id = nid;

    vec1f lambda, flux;

    if (component.empty()) { //sum bulge, disk and agn components
        // Read lookup table
        note("No component specified, return bulge+disk+agn");
        vec1u tstart_bulge, tnbyte_bulge, tstart_disk, tnbyte_disk, tstart_agn, tnbyte_agn;
        fits::read_table(file::remove_extension(seds)+"-lookup.fits",
                         "bulge_start", tstart_bulge, "bulge_nbyte", tnbyte_bulge,
                         "disk_start",  tstart_disk,  "disk_nbyte",  tnbyte_disk,
                         "agn_start",   tstart_agn,   "agn_nbyte",   tnbyte_agn
                         );
        
        std::ifstream file(seds);
        
        // Read bulge
        uint_t start = tstart_bulge[id];
        uint_t nbyte = tnbyte_bulge[id]/2;
        uint_t npt = nbyte/sizeof(float);
        
        vec1f lambda1(npt), flux1(npt);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda1.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux1.data.data()), nbyte);
        
        // Read disk
        start = tstart_disk[id];
        nbyte = tnbyte_disk[id]/2;
        npt = nbyte/sizeof(float);
        
        vec1f lambda2(npt), flux2(npt);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda2.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux2.data.data()), nbyte);
        
        // Read agn
        start = tstart_agn[id];
        nbyte = tnbyte_agn[id]/2;
        npt = nbyte/sizeof(float);
        
        vec1f lambda3(npt), flux3(npt);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda3.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux3.data.data()), nbyte);
        
        // Sum them up
        vec1f lambdag(npt), fluxg(npt);
        merge_add(lambda1, lambda2, flux1, flux2, lambdag, fluxg);
        merge_add(lambdag, lambda3, fluxg, flux3, lambda, flux);
        
    } else if (component == "galaxy") { //sum bulge and disk components
        note("Component = galaxy, return bulge+disk");
        // Read lookup table
        vec1u tstart_bulge, tnbyte_bulge, tstart_disk, tnbyte_disk;
        fits::read_table(file::remove_extension(seds)+"-lookup.fits",
                         "bulge_start", tstart_bulge, "bulge_nbyte", tnbyte_bulge,
                         "disk_start",  tstart_disk,  "disk_nbyte",  tnbyte_disk
                         );
        
        std::ifstream file(seds);
        
        // Read bulge
        uint_t start = tstart_bulge[id];
        uint_t nbyte = tnbyte_bulge[id]/2;
        uint_t npt = nbyte/sizeof(float);
        
        vec1f lambda1(npt), flux1(npt);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda1.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux1.data.data()), nbyte);
        
        // Read disk
        start = tstart_disk[id];
        nbyte = tnbyte_disk[id]/2;
        npt = nbyte/sizeof(float);
        
        vec1f lambda2(npt), flux2(npt);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda2.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux2.data.data()), nbyte);
        
        // Sum them up
        merge_add(lambda1, lambda2, flux1, flux2, lambda, flux);
        
    } else { //get only 1 component (bulge/disk/agn)
        note("Component = bulge/disk/agn");
        // Read lookup table
        vec1u tstart, tnbyte;
        fits::read_table(file::remove_extension(seds)+"-lookup.fits",
                         component+"_start", tstart, component+"_nbyte", tnbyte
                         );
        
        // Read component
        uint_t start = tstart[id];
        uint_t nbyte = tnbyte[id]/2;
        uint_t npt = nbyte/sizeof(float);
        
        lambda.resize(npt);
        flux.resize(npt);
        
        std::ifstream file(seds);
        file.seekg(start);
        file.read(reinterpret_cast<char*>(lambda.data.data()), nbyte);
        file.read(reinterpret_cast<char*>(flux.data.data()), nbyte);
    }


    if (ascii) {
        ascii::output_format opts;
        opts.header = {"lambda[um]", "flux[uJy]"};
        ascii::write_table(out, opts, lambda, format::scientific(flux));
    } else {
        fits::write_table(out, ftable(lambda, flux));
    }

    return 0;
}

void print_help() {
    using namespace format;

    auto argdoc = [](const std::string& name, const std::string& type,
        const std::string& desc) {

        std::string header = " - "+name+" "+type;
        print(header);

        std::string indent = "    ";
        vec1s w = wrap(indent+desc, 80, indent);

        for (auto& s : w) {
            print(s);
        }
    };

    print("egg-getsed v1.0.9, modified by Xavi to include agn and galaxy component");
    print("usage: egg-getsed [options]\n");

    print("List of options:");
    argdoc("seds", "[string]", "file containing the SEDs (mandatory)");
    argdoc("id", "[uint]", "ID of the galaxy to extract (matching the ID column of the "
        "generated catalog, mandatory) ");
    argdoc("component", "[string]", "name of the galaxy component to extract (disk, bulge, agn or galaxy(disk+bulge)); "
        "if not provided then the total SED of the galaxy (disk+bulge+agn) is extracted");
    argdoc("ascii", "[flag]", "set this flag to save an ASCII table instead of FITS");
    argdoc("out", "[string]", "FITS file in which the SED will be extracted (default: "
        "[out]-[component]-[id].fits/cat)");
    print("");
}
