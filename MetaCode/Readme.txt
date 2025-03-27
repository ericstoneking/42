This "MetaCode" folder hosts code that writes code.  Julia scripts read through selected header files, building two json dictionaries (42.json and Ac.json).  The original intent was to read these into CCDD, a free database program that has ties to the open-source Core Flight System (cFS) project.  That interface never happened, but we found other uses for the json files.  Current uses are: 
   1) Autocoding the socket interface for Standalone functionality (see the Standalone folder for more), 2) Autocoding the more general Inter-Process Communication (IPC) C code (see Tx and Rx folders) 3) Automating data outputs via csv files, and 4) Making Ac variables available to the command script functionality (under construction).

The julia and python scripts are:
   HeadersToJson.jl:  Builds the json dictionaries from header files, using 
      the header syntax and marked-up comments to define various properties.
      See format hints below.
   JsonToTxRxIPC.py:  Uses the json dictionary to build C code that passes
      chosen variables to and fro over sockets.  Much of this code is 
      repetitive, which is why it's attractive to use scripts to write it.
   JsonToAcCmdInterpreter.py:  Uses the json dictionaries to build C code to
      enable setting variables in the Ac struct from the command script.  
      This script is deprecated, and will be replaced by a julia script.
   JsonToScCsv.jl, JsonToAcCsv.jl:  Writes C code to output variables from 
      the truth model side (Sc) and ACS side (Ac) to csv files, grouped by 
      struct and metered by the input files Inp_ScOutput.txt and 
      Inp_AcOutput.txt.
   JsonToAcIPC.jl:  Writes C code for the socket interface between 42 and AcApp
      for the Standalone configuration.

Going forward, we are migrating away from python toward julia.


Header formatting:

The julia script HeadersToJson.jl needs the headers to follow some formatting rules.  Some of these are just how I write C anyway, some have been adopted specifically to make the headers easier for julia to parse.  The example below has all of the recognized markups that the julia script will use:

/////////////////////////  Begin Example  //////////////////////
struct FooType {
   /*~ Command Parameters ~*/
   long Cmd1;
   double CmdVec[3];
   double CmdMtx[3][5];
   double CommandableVar; /* [~!~] */

   /*~ Table Parameters ~*/
   long Tbl1;
   double TblVec[3];
   double TblMtx[3][5];

   /*~ Inputs ~*/
   long Nin;
   double In1;
   double In2;

   /*~ Outputs ~*/
   double Out1;
   double Out2;
   double TxVar; /* [~>~] */
   double RxVar; /* [~<~] */
   double TxRxVar; /* [~=~] */

   /*~ Diagnostics ~*/
   double Diag1; /* [[furlongs]] */
   double Diag2; /* [[fortnights]] */

   /*~ Internal Variables ~*/
   double Internal;

   /*~ Structures ~*/
   struct AcType AC;
   struct BarType *Bar;  /* [*Nbar*] */
};

/*~ Prototypes ~*/
void FooFunc(void);

///////////////////////////////  End Example  ///////////////////

The struct keyword and its closing curly brace need to be in the first column.
Only one variable definition per line, please.

By custom, I use only a limited subset of C variable types.  If you add a variable that is not in the following list, you'll need to add it to the list of recognized types in HeadersToJson.jl: long, double, char, float, unsigned char, unsigned int, FILE, struct.

The section partitions (Command Parameters, Table Parameters, Inputs, Outputs, Diagnostics, and Internal Variables) correspond to traditional command/telemetry packet roles.  The Command, Table, Input, and Output categories are important to the Standalone interface.  The Prototypes tag is to signal to the python script that all structure definitions have been read.

Any trailing comment gets copied into the dictionary as a description.  Some key 
character combinations have special meaning:
   [[unit]] pulls the units out of the comment
   [*Nbar*] tells me how many Bars there are.  When an array is defined with
      a fixed size, the python can read that off directly.  But when the 
      array is to be dynamically allocated, I need a symbolic key for loop 
      construction.
   [~ ~] delimiters bracket some combination of the characters <>=!
       > means that we'll be sending this variable from 42 to something else.
       < means that we'll be receiving this variable to 42 from something else.
       = means that 42 will both send and receive this variable.
       ! means that 42 can read this variable from a command file in the style
         of Inp_Cmd.txt

The last set (>,<,=,!) is used for interprocess comm (IPC).  Think of 42 being on your left, and some other process (like a telemetry broadcaster) on your right.  ">" goes from left to right, "<" goes from right to left, and "=" is both ways.  (Sometimes two separate instances of 42 are on either end of a socket, so the = case is not uncommon.)  "!" is a special case, since the command file has its own rules.

As shipped, 42.json, Ac.json, and the subsequent auto-gen C code are already included for convenience.  There are some commented-out lines in the Makefile that, if uncommented, will run the proper scripts to keep the auto-gen code up to date with the headers.  These lines are commented out so that you aren't forced into a dependence on julia unless and until you want it.

