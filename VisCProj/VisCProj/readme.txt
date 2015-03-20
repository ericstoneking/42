To Microsoft Visual C++ 2010 Express developers:

Once you have checked out 42 and 42ExternalSupport from the repository

https://yourusernamehere@gs-mesavs4.ndc.nasa.gov/svn/ACSEB

you can start building the 42 VC project. 

In order to successfully build this project and obtain the final output 42.exe 
under folder 42 (parent folder of VisCProj), one needs to create a folder "External" 
under folder 42, and then copy all contents in the GLEE and freeglut from the 
42ExternalSupport folder. So that the structure looks like the following (excerpt):
\---42
    +---Analysis
    |   \---Thruster Selection
    +---ARC
    +---BBM
    +---CMG
    +---Debug
    +---Development
    +---Docs
    +---External
    |   +---freeglut
    |   |   +---include
    |   |   |   \---GL
    |   |   \---lib
    |   \---GLEE
Make sure GLee.c and GLee.h are in the GLEE folder; 
freeglut.lib and freeglut_static.lib are in the freeglut/lib folder; 
freeglut.h & glut.h are in the freeglut/include/GL folder. 
Also, copy the freeglut.dll to the output directory, 42 folder. 
42.exe needs freeglut.dll at runtime. 