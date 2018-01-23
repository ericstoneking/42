##########################  Macro Definitions  ############################

# Let's try to auto-detect what platform we're on.  
# If this fails, set 42PLATFORM manually in the else block.
AUTOPLATFORM = Failed
ifeq ($(PLATFORM),apple)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __APPLE__
endif
ifeq ($(PLATFORM),linux)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __linux__
endif
ifeq ($(MSYSTEM),MINGW32)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __MSYS__
endif
ifeq ($(AUTOPLATFORM),Failed)
   # Autodetect failed.  Set platform manually.
   42PLATFORM = __APPLE__
   #42PLATFORM = __linux__
   #42PLATFORM = __MSYS__
endif

GUIFLAG = -D _USE_GUI_
#GUIFLAG = 

SHADERFLAG = -D _USE_SHADERS_
#SHADERFLAG = 

#TIMEFLAG = 
TIMEFLAG = -D _USE_SYSTEM_TIME_

CFDFLAG = 
#CFDFLAG = -D _ENABLE_CFD_SLOSH_

FFTBFLAG = 
#FFTBFLAG = -D _ENABLE_FFTB_CODE_

#GSFCFLAG = 
GSFCFLAG = -D _USE_GSFC_WATERMARK_

# Basic directories
HOMEDIR = ./
PROJDIR = ./
KITDIR = $(PROJDIR)Kit/
OBJ = $(PROJDIR)Object/
INC = $(PROJDIR)Include/
SRC = $(PROJDIR)Source/
KITINC = $(KITDIR)Include/
KITSRC = $(KITDIR)Source/
INOUT = $(PROJDIR)InOut/
PRIVSRC = $(PROJDIR)/Private/Source/

#EMBEDDED = -D EMBEDDED_MATLAB
EMBEDDED = 

ifneq ($(strip $(EMBEDDED)),)
   MATLABROOT = "C:/Program Files/MATLAB/R2010b/"
   MATLABINC = $(MATLABROOT)extern/include/ 
   SIMULINKINC = $(MATLABROOT)simulink/include/
   MATLABLIB = -leng -lmx -lmwmathutil
   MATLABSRC = $(PROJDIR)External/MATLABSRC/
else
   MATLABROOT =  
   MATLABINC =  
   SIMULINKINC = 
   MATLABLIB = 
   MATLABSRC = 
endif

ifeq ($(42PLATFORM),__APPLE__)
   # Mac Macros
   CINC = -I /usr/include
   EXTERNDIR = 
   GLINC = -I /System/Library/Frameworks/OpenGL.framework/Headers/ -I /System/Library/Frameworks/GLUT.framework/Headers/
   # ARCHFLAG = -arch i386
   ARCHFLAG = -arch x86_64

   #SOCKETFLAG = 
   SOCKETFLAG = -D _ENABLE_SOCKETS_

   ifneq ($(strip $(EMBEDDED)),)
      LFLAGS = -bind_at_load -m64 -L$(MATLABROOT)bin/maci64
      LIBS = -framework System -framework Carbon -framework OpenGL -framework GLUT $(MATLABLIB)
   else
      LFLAGS = -bind_at_load
      LIBS = -framework System -framework Carbon -framework OpenGL -framework GLUT
   endif
   GUIOBJ = $(OBJ)42GlutGui.o $(OBJ)glkit.o 
   EXENAME = 42
   CC = gcc
endif

ifeq ($(42PLATFORM),__linux__)
   # Linux Macros
   CINC =
   EXTERNDIR =  

   #SOCKETFLAG = 
   SOCKETFLAG = -D _ENABLE_SOCKETS_

   ifneq ($(strip $(GUIFLAG)),)
      GUIOBJ = $(OBJ)42GlutGui.o $(OBJ)glkit.o 
      #GLINC = -I /usr/include/
      GLINC = -I $(KITDIR)/include/GL/
      LIBS = -lglut -lGLU -lGL 
      LFLAGS = -L $(KITDIR)/GL/lib/
      ARCHFLAG = 
   else
      GUIOBJ = 
      GLINC = 
      LIBS = 
      LFLAGS = 
      ARCHFLAG = 
   endif
   EXENAME = 42
   CC = g++
endif

ifeq ($(42PLATFORM),__MSYS__)
   CINC = 
   EXTERNDIR = /c/42ExternalSupport/

   #SOCKETFLAG = 
   SOCKETFLAG = -D _ENABLE_SOCKETS_

   ifneq ($(strip $(GUIFLAG)),)
      GLEW = $(EXTERNDIR)GLEW/
      GLUT = $(EXTERNDIR)freeglut/
      LIBS =  -lopengl32 -lglu32 -lfreeglut -lws2_32 -lglew32
      LFLAGS = -L $(GLUT)lib/ -L $(GLEW)lib/
      GUIOBJ = $(OBJ)42GlutGui.o $(OBJ)glkit.o 
      GLINC = -I $(GLEW)include/GL/ -I $(GLUT)include/GL/
      ARCHFLAG = -D GLUT_NO_LIB_PRAGMA -D GLUT_NO_WARNING_DISABLE -D GLUT_DISABLE_ATEXIT_HACK
   else
      GUIOBJ = 
      GLINC = 
      LIBS = 
      LFLAGS = 
      ARCHFLAG = 
   endif
   ifneq ($(strip $(EMBEDDED)),)
      LFLAGS = -L $(GLUT)lib/ -m64 -L$(MATLABROOT)bin/win32
      LIBS = -lopengl32 -lglu32 -lfreeglut $(MATLABLIB) 
   endif
   EXENAME = 42.exe
   CC = gcc
endif

# If not using GUI, don't compile GUI-related files
ifeq ($(strip $(GUIFLAG)),)
   GUIOBJ = 
endif

# If not using IPC, don't compile IPC-related files
ifneq ($(strip $(SOCKETFLAG)),)
   IPCOBJ = $(OBJ)42ipc.o
else
   IPCOBJ = 
endif

# If not in FFTB, don't compile FFTB-related files
ifneq ($(strip $(FFTBFLAG)),)
   FFTBOBJ = $(OBJ)42fftb.o
else
   FFTBOBJ = 
endif

#ANSIFLAGS = -Wstrict-prototypes -pedantic -ansi -Werror
ANSIFLAGS = 

CFLAGS = -Wall -Wshadow -Wno-deprecated -g  $(ANSIFLAGS) $(GLINC) $(CINC) -I $(INC) -I $(KITINC) -I $(KITSRC) -I $(MATLABSRC) -I $(MATLABINC) -I $(SIMULINKINC) -O0 $(ARCHFLAG) $(GUIFLAG) $(SHADERFLAG) $(TIMEFLAG) $(SOCKETFLAG) $(EMBEDDED) $(CFDFLAG) $(FFTBFLAG) $(GSFCFLAG)

42OBJ = $(OBJ)42main.o $(OBJ)42exec.o $(OBJ)42actuators.o $(OBJ)42cmd.o \
$(OBJ)42dynam.o $(OBJ)42environs.o $(OBJ)42ephem.o $(OBJ)42fsw.o \
$(OBJ)42init.o $(OBJ)42perturb.o $(OBJ)42report.o \
$(OBJ)42sensors.o 

ifneq ($(strip $(CFDFLAG)),)
   SLOSHOBJ = $(OBJ)42CfdSlosh.o
else
   SLOSHOBJ = 
endif

KITOBJ = $(OBJ)dcmkit.o $(OBJ)envkit.o $(OBJ)fswkit.o $(OBJ)geomkit.o \
$(OBJ)iokit.o $(OBJ)mathkit.o $(OBJ)nrlmsise00kit.o $(OBJ)msis86kit.o $(OBJ)orbkit.o $(OBJ)radbeltkit.o $(OBJ)sigkit.o $(OBJ)sphkit.o $(OBJ)timekit.o

ifneq ($(strip $(EMBEDDED)),)
   MATLABOBJ = $(OBJ)DetectorFSW.o $(OBJ)OpticsFSW.o
else
   MATLABOBJ = 
endif

##########################  Rules to link 42  #############################

42 : $(42OBJ) $(GUIOBJ) $(IPCOBJ) $(FFTBOBJ) $(SLOSHOBJ) $(KITOBJ) $(MATLABOBJ)
	$(CC) $(LFLAGS) -o $(EXENAME) $(42OBJ) $(GUIOBJ) $(IPCOBJ) $(FFTBOBJ) $(SLOSHOBJ) $(KITOBJ) $(MATLABOBJ) $(LIBS)  

####################  Rules to compile objects  ###########################

$(OBJ)42main.o          : $(SRC)42main.c 
	$(CC) $(CFLAGS) -c $(SRC)42main.c -o $(OBJ)42main.o  

$(OBJ)42exec.o          : $(SRC)42exec.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42exec.c -o $(OBJ)42exec.o  

$(OBJ)42actuators.o : $(SRC)42actuators.c $(INC)42.h $(INC)42fsw.h  $(INC)fswdefines.h $(INC)fswtypes.h
	$(CC) $(CFLAGS) -c $(SRC)42actuators.c -o $(OBJ)42actuators.o  

$(OBJ)42cmd.o : $(SRC)42cmd.c $(INC)42.h $(INC)42fsw.h  $(INC)fswdefines.h $(INC)fswtypes.h
	$(CC) $(CFLAGS) -c $(SRC)42cmd.c -o $(OBJ)42cmd.o  

$(OBJ)42dynam.o     : $(SRC)42dynam.c $(INC)42.h 
	$(CC) $(CFLAGS) -c $(SRC)42dynam.c -o $(OBJ)42dynam.o  

$(OBJ)42environs.o  : $(SRC)42environs.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42environs.c -o $(OBJ)42environs.o  

$(OBJ)42ephem.o     : $(SRC)42ephem.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42ephem.c -o $(OBJ)42ephem.o  

$(OBJ)42fsw.o       : $(SRC)42fsw.c $(INC)42fsw.h $(INC)fswdefines.h $(INC)fswtypes.h 
	$(CC) $(CFLAGS) -c $(SRC)42fsw.c -o $(OBJ)42fsw.o  

$(OBJ)42GlutGui.o        : $(SRC)42GlutGui.c $(INC)42.h $(INC)42GlutGui.h 
	$(CC) $(CFLAGS) -c $(SRC)42GlutGui.c -o $(OBJ)42GlutGui.o  

$(OBJ)42init.o      : $(SRC)42init.c $(INC)42.h  
	$(CC) $(CFLAGS) -c $(SRC)42init.c -o $(OBJ)42init.o  

$(OBJ)42ipc.o       : $(SRC)42ipc.c $(INC)42.h  
	$(CC) $(CFLAGS) -c $(SRC)42ipc.c -o $(OBJ)42ipc.o 

$(OBJ)42perturb.o   : $(SRC)42perturb.c $(INC)42.h 
	$(CC) $(CFLAGS) -c $(SRC)42perturb.c -o $(OBJ)42perturb.o  

$(OBJ)42report.o    : $(SRC)42report.c $(INC)42.h 
	$(CC) $(CFLAGS) -c $(SRC)42report.c -o $(OBJ)42report.o  

$(OBJ)42sensors.o   : $(SRC)42sensors.c $(INC)42.h $(INC)42fsw.h $(INC)fswdefines.h $(INC)fswtypes.h
	$(CC) $(CFLAGS) -c $(SRC)42sensors.c -o $(OBJ)42sensors.o  

$(OBJ)dcmkit.o      : $(KITSRC)dcmkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)dcmkit.c -o $(OBJ)dcmkit.o  

$(OBJ)envkit.o      : $(KITSRC)envkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)envkit.c -o $(OBJ)envkit.o  

$(OBJ)fswkit.o      : $(KITSRC)fswkit.c 
	$(CC) $(CFLAGS) -c $(KITSRC)fswkit.c -o $(OBJ)fswkit.o  

$(OBJ)glkit.o      : $(KITSRC)glkit.c $(KITINC)glkit.h
	$(CC) $(CFLAGS) -c $(KITSRC)glkit.c -o $(OBJ)glkit.o  

$(OBJ)geomkit.o      : $(KITSRC)geomkit.c $(KITINC)geomkit.h 
	$(CC) $(CFLAGS) -c $(KITSRC)geomkit.c -o $(OBJ)geomkit.o  

$(OBJ)iokit.o      : $(KITSRC)iokit.c 
	$(CC) $(CFLAGS) -c $(KITSRC)iokit.c -o $(OBJ)iokit.o  

$(OBJ)mathkit.o     : $(KITSRC)mathkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)mathkit.c -o $(OBJ)mathkit.o  

$(OBJ)nrlmsise00kit.o   : $(KITSRC)nrlmsise00kit.c  
	$(CC) $(CFLAGS) -c $(KITSRC)nrlmsise00kit.c -o $(OBJ)nrlmsise00kit.o  

$(OBJ)msis86kit.o   : $(KITSRC)msis86kit.c $(KITINC)msis86kit.h 
	$(CC) $(CFLAGS) -c $(KITSRC)msis86kit.c -o $(OBJ)msis86kit.o  

$(OBJ)orbkit.o      : $(KITSRC)orbkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)orbkit.c -o $(OBJ)orbkit.o  

$(OBJ)radbeltkit.o      : $(KITSRC)radbeltkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)radbeltkit.c -o $(OBJ)radbeltkit.o  

$(OBJ)sigkit.o      : $(KITSRC)sigkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)sigkit.c -o $(OBJ)sigkit.o  

$(OBJ)sphkit.o      : $(KITSRC)sphkit.c
	$(CC) $(CFLAGS) -c $(KITSRC)sphkit.c -o $(OBJ)sphkit.o  

$(OBJ)timekit.o     : $(KITSRC)timekit.c 
	$(CC) $(CFLAGS) -c $(KITSRC)timekit.c -o $(OBJ)timekit.o  

$(OBJ)DetectorFSW.o	: $(MATLABSRC)DetectorFSW.c
	$(CC) $(CFLAGS) -c $(MATLABSRC)DetectorFSW.c -o $(OBJ)DetectorFSW.o

$(OBJ)OpticsFSW.o	: $(MATLABSRC)OpticsFSW.c
	$(CC) $(CFLAGS) -c $(MATLABSRC)OpticsFSW.c -o $(OBJ)OpticsFSW.o

$(OBJ)42CfdSlosh.o      : $(PRIVSRC)42CfdSlosh.c $(INC)42.h   
	$(CC) $(CFLAGS) -c $(PRIVSRC)42CfdSlosh.c -o $(OBJ)42CfdSlosh.o  


$(OBJ)42fftb.o      : $(PRIVSRC)42fftb.c $(INC)42.h   
	$(CC) $(CFLAGS) -c $(PRIVSRC)42fftb.c -o $(OBJ)42fftb.o  

########################  Miscellaneous Rules  ############################
clean :
ifeq ($(42PLATFORM),_WIN32)
	del .\Object\*.o .\$(EXENAME) .\InOut\*.42
else ifeq ($(42PLATFORM),_WIN64)
	del .\Object\*.o .\$(EXENAME) .\InOut\*.42
else
	rm $(OBJ)*.o ./$(EXENAME) $(INOUT)*.42 ./Demo/*.42 ./Rx/*.42 ./Tx/*.42
endif


