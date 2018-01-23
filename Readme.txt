Welcome to 42, the mostly harmless spacecraft simulation tool.

See the overview, "42 Overview.pdf", in the Docs folder.  "Nomenclature.pdf", "POV Menu.pdf", and "Key Bindings.txt" are also recommended.

If you're installing on Windows, see the file "Install-msys.txt" in the Docs folder.

The compiler will attempt to detect what platform you're on (Linux, OSX, or Msys), but its success rate isn't great.  If you have errors on the first compile or run, try editing your Makefile to manually set your 42PLATFORM.  

The default folder for inputs and outputs is "InOut".  Look there for sample input files.  "Inp_Sim.txt" is the top-level input file.

The input/output folder may be changed for a sim run by running 42 with an argument.  For example, enter this at the command prompt:
42 Demo

The VisCProj and XcodeProj folders are provided as starting points.  I haven't kept them up to date as 42 has evolved.  The Makefile is current.

