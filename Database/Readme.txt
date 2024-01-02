This "Database" folder serves as the interface between 42 and some database functionality.  Python scripts read through selected header files, building a json dictionary (42.json).  This json dictionary can be read into CCDD, a free database program that has ties to the open-source Core Flight System (cFS) project.  Aside from this interface, we also use 42.json to generate some Inter-Process Communication (IPC) C code that would be otherwise tedious and repetitive.  This code has to do with passing variables across sockets, message buses, and the like.

The python scripts are:
   HeadersToJson.py:  Builds the json dictionary from header files, using 
      the header syntax and marked-up comments to define various properties.
   JsonToIPC.py:  Uses the json dictionary to build C code that passes
      chosen variables to and fro over sockets, message buses, file dumps, etc.
      Much of this code is repetitive, which is why it's attractive to use
      python to write it.


Header formatting:

The python script HeadersToJson.py needs the headers to follow some formatting rules.  Some of these are just how I write C anyway, some have been adopted specifically to make the python easier to write.  The example below has all of the recognized markups that the python script will use:

/////////////////////////  Begin Example  //////////////////////
struct FooType {
   /*~ Parameters ~*/
   long Prm1;
   double PrmVec[3];
   double PrmMtx[3][5];

   /*~ Inputs ~*/
   long Nin;
   double In1;
   double In2;

   /*~ Outputs ~*/
   double Out1;
   double Out2;

   /*~ Diagnostics ~*/
   double Diag1; /* [[furlongs]] */
   double Diag2; /* [[fortnights]] */

   /*~ Internal Variables ~*/
   double Internal;
   double TxVar; /* [~>~] */
   double RxVar; /* [~<~] */
   double TxRxVar; /* [~=~] */
   double CommandableVar; /* [~!~] */
   double TxRxCmdVar; /* [~=!~]

   /*~ Structures ~*/
   struct AcType AC;
   struct BarType *Bar;  /* [*Nbar*] */
};

/*~ Prototypes ~*/
void FooFunc(void);

///////////////////////////////  End Example  ///////////////////

The struct keyword and its closing curly brace need to be in the first column.
Only one variable definition per line, please.

By custom, I use only a limited subset of C variable types.  If you add a variable that is not in the following list, you'll need to add it to the list of recognized types in HeadersToJson.py: long, double, char, float, unsigned char, unsigned int, FILE, struct.

The section partitions (Parameters, Inputs, Outputs, Diagnostics, and Internal Variables) correspond to traditional command/telemetry packet roles.  I don't use them (much) myself, but CCDD and its adherents do.  The Prototypes tag is to signal to the python script that all structure definitions have been read.

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

The last set (>,<,=,!) is used for interprocess comm (IPC): sockets, message buses, and file dumps.  Think of 42 being on your left, and some other app (like the ACS standalone app) on your right.  ">" goes from left to right, "<" goes from right to left, and "=" is both ways.  (Sometimes two separate instances of 42 are on either end of a socket, so the = case is not uncommon.)  "!" is a special case, since the command file has its own rules.

As shipped, 42.json and the subsequent auto-gen C code are already included for convenience.
