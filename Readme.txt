Welcome to 42, the mostly harmless spacecraft simulation tool.

See the overview, "42 Overview.pdf", in the Docs folder. Also recommended:
Nomenclature.pdf
POV Menu.pdf
Key Bindings.txt
FSW Models.pdf
Flight Regimes.pdf

If you're installing on Windows, see the file "Install-msys.txt" in the Docs folder.

The compiler will attempt to detect what platform you're on (Linux, OSX, or Msys), but its success rate isn't great.  If you have errors on the first compile or run, try editing your Makefile to manually set your 42PLATFORM.  

The default folder for inputs and outputs is "InOut".  Look there for sample input files.  "Inp_Sim.txt" is the top-level input file.

The input/output folder may be changed for a sim run by running 42 with an argument.  For example, enter this at the command prompt:
42 Demo


Common Problems:
1)  42 expects the input files to be plain vanilla text files.  If your text editor adds formatting, makes straight quotes into smart quotes, etc, 42 may get confused.  The most common symptom is generating the "Bogus input in DecodeString" error.
2)  Also text-related, 42 is very simple-minded about reading the input files.  Adding extra lines, or accidentally deleting a line, or swapping lines, will throw things out of synch.  Again, the most common symptom is the "Bogus input in DecodeString" error.  Use your debugger to trace back where the error was generating.  (The actual mistake may be at that line, or may be somewhere upstream.)

