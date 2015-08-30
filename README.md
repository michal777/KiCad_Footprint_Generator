# KiCad_Footprint_Generator

src:
- KiCad_Footprint_Generator - main, with csv parser
- Footprint - general class of footprint
- all the rest - classes of different footprints, algorithms used to draw them

Debug:
- KiCad_Footprint_Generator - executable compiled on linux
- all the rest - output from eclipse

Data:
- generate_packages - script executing KiCad_Footprint_Generator on all csv
- SYMLINK_KiCad_Footprint_Generator - link to ../Debug/KiCad_Footprint_Generator
- pack_param_csv - parameters in csv (separator - colon, decimal point - dot)
- footprints - generated footprints

