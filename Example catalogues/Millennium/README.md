# MAMBO AGN Mock
Created by Xavier Lopez Lopez, Lucia Pozzetti and Micol Bolzonella
- This lightcone is based on a SHMR applied to DM halo and sub-halos from the Millennium simulation (Henriques 2015 lightcones)
- Each galaxy in the mock is made of 2 components: disk and bulge, each one with a different SED assigned.
AGN (type 1/2) have an extra component. CTK AGN (log NH > 24) are not included.- -AGN are separated into optically obscured/unobscured (type 2/1 respectively) following Merloni et al. 2014.
- We include SEDs for: 200 type 1 AGN, 200 type 2 AGN, 150 SF galaxies and 50 quiescent galaxies, all of them randomly selected within the full lightcone. For each object, we provide 3 SEDs: the AGN component, the host galaxy component (bulge+disk), and the sum of them.
- Relevant galaxy/AGN parameters:
z: redshift
RA, DEC
M:* Total stellar mass of the galaxy, Salpeter IMF
M Disk:* Stellar mass of the disk component
M Bulge:* Stellar mass of the disk component
Quiescent/ Star-forming: Galaxy type flag, 0 = SF, 1 = Q
SFR: Star formation rate (SFR).
RSB: “starburstiness”, i.e., the ratio between the SFR of the galaxy and the SFR of the Main Sequence at the redshift and the stellar mass of this galaxy.
SFRIR, SFRUV: Contribution of the IR and UV light to the total SFR of the galaxy.
Metallicity
Disk angle, bulge angle: Position angle of each component on the sky
Disk radius, bulge radius: Angular size of each component, in arcseconds
Disk ratio, bulge ratio: Axis ratio of each stellar component: 1 is perfectly round, 0 (which never happens) is perfectly linear.
Av disk, Av bulge: Attenuation for each component
Broad band photometry: observed flux and absolute magnitude for each component
Emission line fluxes and velocity dispersion
SED from NIR to UV
- AGN parameters:
Type flag: 1/2: optical obscuration
X-ray luminosity : intrinsic, hard band [2-10 keV]
BH mass
Av qso : Attenuation for the qso SED
NH:  Absorption column density in the X-ray
Emission line fluxes and velocity dispersion (only narrow lines of type 2 )
Physical properties of NLR of type 2: metallicity, logU, log nH
Emission line fluxes of type 1 (very soon, work in progress) (broad and narrow)
Broad band photometry: observed flux and absolute magnitude for AGN component
SED from NIR to UV available if requested
- List of narrow lines for type 2 AGN:
Balmer, Paschen, NV_1240, CIV_1550, HeII_1640, [CIII]_1908, MgII_2796, MgII_2803, [NeV]_3426, [OII]_3727, [NeIII]_3869, HeII_4686, [OIII]_4959, [OIII]_5007, [OI]_6300, [OI]_6363, [NII]_6548, [NII]_6584, [SII]_6717, [SII]_6731, [SIII]_6312, [SIII]_9069, [SIII]_9532.

