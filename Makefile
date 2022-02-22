##########################  Macro Definitions  ############################

# Let's try to auto-detect what platform we're on.
# If this fails, set 42PLATFORM manually in the else block.
AUTOPLATFORM = Failed
ifeq ($(MSYSTEM),MINGW32)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __MSYS__
endif
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __linux__
endif
ifeq ($(UNAME_S),Darwin)
   AUTOPLATFORM = Succeeded
   42PLATFORM = __APPLE__
endif
ifeq ($(AUTOPLATFORM),Failed)
   # Autodetect failed.  Set platform manually.
   #42PLATFORM = __APPLE__
   #42PLATFORM = __linux__
   42PLATFORM = __MSYS__
endif


GUIFLAG = -D _ENABLE_GUI_
#GUIFLAG =

SHADERFLAG = -D _USE_SHADERS_
#SHADERFLAG =

CFDFLAG =
#CFDFLAG = -D _ENABLE_CFD_SLOSH_

FFTBFLAG =
#FFTBFLAG = -D _ENABLE_FFTB_CODE_

GSFCFLAG =
#GSFCFLAG = -D _USE_GSFC_WATERMARK_

STANDALONEFLAG =
#STANDALONEFLAG = -D _AC_STANDALONE_

GMSECFLAG =
#GMSECFLAG = -D _ENABLE_GMSEC_

ifeq ($(strip $(GMSECFLAG)),)
   GMSECDIR =
   GMSECINC =
   GMSECBIN =
   GMSECLIB =
else
   GMSECDIR = ~/GMSEC/
   GMSECINC = -I $(GMSECDIR)include/
   GMSECBIN = -L $(GMSECDIR)bin/
   GMSECLIB = -lGMSECAPI
endif

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
GSFCSRC = $(PROJDIR)/GSFC/Source/
IPCSRC = $(SRC)IPC/

ifeq ($(42PLATFORM),__APPLE__)
   # Mac Macros
   CINC = -I /usr/include -I /usr/local/include
   EXTERNDIR =
   # ARCHFLAG = -arch i386
   ARCHFLAG = -arch x86_64
   # For graphics interface, choose GLUT or GLFW GUI libraries
   # GLUT is well known, but GLFW is better for newer Mac's hires displays
   #GLUT_OR_GLFW = _USE_GLFW_
   GLUT_OR_GLFW = _USE_GLUT_

   LFLAGS = -bind_at_load
   ifneq ($(strip $(GUIFLAG)),)
      GLINC = -I /System/Library/Frameworks/OpenGL.framework/Headers/ -I /System/Library/Frameworks/GLUT.framework/Headers/
      ifeq ($(strip $(GLUT_OR_GLFW)),_USE_GLUT_)
         LIBS = -framework System -framework Carbon -framework OpenGL -framework GLUT
         GUIOBJ = $(OBJ)42gl.o $(OBJ)42glut.o $(OBJ)glkit.o $(OBJ)42gpgpu.o
         GUI_LIB = -D _USE_GLUT_
      else
         LIBS = -lglfw -framework System -framework Carbon -framework OpenGL -framework GLUT
         GUIOBJ = $(OBJ)42gl.o $(OBJ)42glfw.o $(OBJ)glkit.o $(OBJ)42gpgpu.o
         GUI_LIB = -D _USE_GLFW_
      endif
   else
      GLINC = 
      LIBS = 
      GUIOBJ = 
   endif
   XWARN = 
   EXENAME = 42
   CC = gcc
endif

ifeq ($(42PLATFORM),__linux__)
   # Linux Macros
   CINC =
   EXTERNDIR =
   ARCHFLAG =
   # For graphics interface, choose GLUT or GLFW GUI libraries
   # GLUT is well known, but GLFW is better for newer Mac's hires displays
   #GLUT_OR_GLFW = _USE_GLFW_
   GLUT_OR_GLFW = _USE_GLUT_

   ifneq ($(strip $(GUIFLAG)),)
      ifeq ($(strip $(GLUT_OR_GLFW)),_USE_GLUT_)
         GUIOBJ = $(OBJ)42gl.o $(OBJ)42glut.o $(OBJ)glkit.o $(OBJ)42gpgpu.o
         LIBS = -lglut -lGLU -lGL -ldl -lm -lpthread
         GLINC = -I /usr/include/GL/
         LFLAGS = -L $(KITDIR)/GL/lib/
         GUI_LIB = -D _USE_GLUT_
      else
         GUIOBJ = $(OBJ)42gl.o $(OBJ)42glfw.o $(OBJ)glkit.o $(OBJ)42gpgpu.o
         LIBS = -lglfw -lglut -lGLU -lGL -ldl -lm -lpthread
         GLINC = -I /usr/include/GL/ -I /usr/include/GLFW
         GUI_LIB = -D _USE_GLFW_
      endif
   else
      GUIOBJ =
      GLINC =
      LIBS = -ldl -lm -lpthread
      LFLAGS =
   endif
   XWARN = -Wno-unused-variable -Wno-unused-but-set-variable -Wno-stringop-overread
   EXENAME = 42
   CC = gcc
endif

ifeq ($(42PLATFORM),__MSYS__)
   CINC =
   EXTERNDIR = /c/42ExternalSupport/
   # For graphics interface, choose GLUT or GLFW GUI libraries
   # GLUT is well known, but GLFW is better for newer Mac's hires displays
   #GLUT_OR_GLFW = _USE_GLFW_
   GLUT_OR_GLFW = _USE_GLUT_

   ifneq ($(strip $(GUIFLAG)),)
      # TODO: Option to use GLFW instead of GLUT?
      GLEW = $(EXTERNDIR)GLEW/
      GLUT = $(EXTERNDIR)freeglut/
      LIBS =  -lopengl32 -lglu32 -lfreeglut -lws2_32 -lglew32
      LFLAGS = -L $(GLUT)lib/ -L $(GLEW)lib/
      GUIOBJ = $(OBJ)42gl.o $(OBJ)42glut.o $(OBJ)glkit.o $(OBJ)42gpgpu.o
      GLINC = -I $(GLEW)include/GL/ -I $(GLUT)include/GL/
      ARCHFLAG = -D GLUT_NO_LIB_PRAGMA -D GLUT_NO_WARNING_DISABLE -D GLUT_DISABLE_ATEXIT_HACK
   else
      GUIOBJ =
      GLINC =
      LIBS =  -lws2_32
      LFLAGS =
      ARCHFLAG =
   endif
   XWARN = 
   EXENAME = 42.exe
   CC = gcc
endif

# If not using GUI, don't compile GUI-related files
ifeq ($(strip $(GUIFLAG)),)
   GUIOBJ =
endif

# If not in FFTB, don't compile FFTB-related files
ifneq ($(strip $(FFTBFLAG)),)
   FFTBOBJ = $(OBJ)42fftb.o
else
   FFTBOBJ =
endif

ifneq ($(strip $(CFDFLAG)),)
   SLOSHOBJ = $(OBJ)42CfdSlosh.o
else
   SLOSHOBJ =
endif

# If not _AC_STANDALONE_, link AcApp.c in with the rest of 42
ifneq ($(strip $(STANDALONEFLAG)),)
   ACOBJ =
else
   ACOBJ = $(OBJ)AcApp.o
endif

ifneq ($(strip $(GMSECFLAG)),)
   GMSECOBJ = $(OBJ)gmseckit.o
   ACIPCOBJ = $(OBJ)AppReadFromFile.o $(OBJ)AppWriteToGmsec.o $(OBJ)AppReadFromGmsec.o \
      $(OBJ)AppWriteToSocket.o $(OBJ)AppReadFromSocket.o $(OBJ)AppWriteToFile.o
   SIMIPCOBJ = $(OBJ)SimWriteToFile.o $(OBJ)SimWriteToGmsec.o $(OBJ)SimWriteToSocket.o \
      $(OBJ)SimReadFromFile.o $(OBJ)SimReadFromGmsec.o $(OBJ)SimReadFromSocket.o 
else
   GMSECOBJ =
   ACIPCOBJ = $(OBJ)AppReadFromFile.o \
      $(OBJ)AppWriteToSocket.o $(OBJ)AppReadFromSocket.o $(OBJ)AppWriteToFile.o
   SIMIPCOBJ = $(OBJ)SimWriteToFile.o $(OBJ)SimWriteToSocket.o \
      $(OBJ)SimReadFromFile.o $(OBJ)SimReadFromSocket.o 
endif

42OBJ = $(OBJ)42main.o $(OBJ)42exec.o $(OBJ)42actuators.o $(OBJ)42cmd.o \
$(OBJ)42dynamics.o $(OBJ)42environs.o $(OBJ)42ephem.o $(OBJ)42fsw.o \
$(OBJ)42init.o $(OBJ)42ipc.o $(OBJ)42perturb.o $(OBJ)42report.o \
$(OBJ)42sensors.o \
$(OBJ)42nos3.o

KITOBJ = $(OBJ)dcmkit.o $(OBJ)envkit.o $(OBJ)fswkit.o $(OBJ)geomkit.o \
$(OBJ)iokit.o $(OBJ)mathkit.o $(OBJ)nrlmsise00kit.o $(OBJ)msis86kit.o \
$(OBJ)orbkit.o $(OBJ)radbeltkit.o $(OBJ)sigkit.o $(OBJ)sphkit.o $(OBJ)timekit.o

LIBKITOBJ = $(OBJ)dcmkit.o $(OBJ)envkit.o $(OBJ)fswkit.o $(OBJ)geomkit.o \
$(OBJ)iokit.o $(OBJ)mathkit.o $(OBJ)orbkit.o $(OBJ)sigkit.o $(OBJ)sphkit.o $(OBJ)timekit.o

ACKITOBJ = $(OBJ)dcmkit.o $(OBJ)mathkit.o $(OBJ)fswkit.o $(OBJ)iokit.o $(OBJ)timekit.o

ACIPCOBJ = $(OBJ)AppReadFromFile.o \
$(OBJ)AppWriteToSocket.o $(OBJ)AppReadFromSocket.o $(OBJ)AppWriteToFile.o

#ANSIFLAGS = -Wstrict-prototypes -pedantic -ansi -Werror
ANSIFLAGS =

CFLAGS = -fpic -Wall -Wshadow -Wno-deprecated $(XWARN) -g  $(ANSIFLAGS) $(GLINC) $(CINC) -I $(INC) -I $(KITINC) -I $(KITSRC) $(GMSECINC) -O0 $(ARCHFLAG) $(GUIFLAG) $(GUI_LIB) $(SHADERFLAG) $(CFDFLAG) $(FFTBFLAG) $(GSFCFLAG) $(GMSECFLAG) $(STANDALONEFLAG)


##########################  Rules to link 42  #############################

42 : $(42OBJ) $(GUIOBJ) $(SIMIPCOBJ) $(FFTBOBJ) $(SLOSHOBJ) $(KITOBJ) $(ACOBJ) $(GMSECOBJ)
	$(CC) $(LFLAGS) $(GMSECBIN) -o $(EXENAME) $(42OBJ) $(GUIOBJ) $(FFTBOBJ) $(SLOSHOBJ) $(KITOBJ) $(ACOBJ) $(GMSECOBJ) $(SIMIPCOBJ) $(LIBS) $(GMSECLIB)

AcApp : $(OBJ)AcApp.o $(ACKITOBJ) $(ACIPCOBJ) $(GMSECOBJ)
	$(CC) $(LFLAGS) -o AcApp $(OBJ)AcApp.o $(ACKITOBJ) $(ACIPCOBJ) $(GMSECOBJ) $(LIBS)
	
libkit : $(LIBKITOBJ)
	$(CC) $(LFLAGS) -shared -o $(KITDIR)libkit.so $(LIBKITOBJ)


####################  Rules to compile objects  ###########################

$(OBJ)42main.o      : $(SRC)42main.c
	$(CC) $(CFLAGS) -c $(SRC)42main.c -o $(OBJ)42main.o

$(OBJ)42exec.o      : $(SRC)42exec.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42exec.c -o $(OBJ)42exec.o

$(OBJ)42actuators.o : $(SRC)42actuators.c $(INC)42.h $(INC)Ac.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(SRC)42actuators.c -o $(OBJ)42actuators.o

$(OBJ)42cmd.o : $(SRC)42cmd.c $(INC)42.h $(INC)Ac.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(SRC)42cmd.c -o $(OBJ)42cmd.o

$(OBJ)42dynamics.o  : $(SRC)42dynamics.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42dynamics.c -o $(OBJ)42dynamics.o

$(OBJ)42environs.o  : $(SRC)42environs.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42environs.c -o $(OBJ)42environs.o

$(OBJ)42ephem.o     : $(SRC)42ephem.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42ephem.c -o $(OBJ)42ephem.o

$(OBJ)42fsw.o       : $(SRC)42fsw.c $(INC)Ac.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(SRC)42fsw.c -o $(OBJ)42fsw.o

$(OBJ)42gl.o        : $(SRC)42gl.c $(INC)42.h $(INC)42gl.h
	$(CC) $(CFLAGS) -c $(SRC)42gl.c -o $(OBJ)42gl.o

$(OBJ)42glfw.o	: $(SRC)42glfw.c $(INC)42.h $(INC)42gl.h $(INC)42glfw.h
	$(CC) $(CFLAGS) -c $(SRC)42glfw.c -o $(OBJ)42glfw.o

$(OBJ)42glut.o      : $(SRC)42glut.c $(INC)42.h $(INC)42gl.h $(INC)42glut.h
	$(CC) $(CFLAGS) -c $(SRC)42glut.c -o $(OBJ)42glut.o

$(OBJ)42gpgpu.o      : $(SRC)42gpgpu.c $(INC)42.h $(INC)42gl.h $(INC)42glut.h
	$(CC) $(CFLAGS) -c $(SRC)42gpgpu.c -o $(OBJ)42gpgpu.o

$(OBJ)42init.o      : $(SRC)42init.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42init.c -o $(OBJ)42init.o

$(OBJ)42ipc.o       : $(SRC)42ipc.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42ipc.c -o $(OBJ)42ipc.o

$(OBJ)42perturb.o   : $(SRC)42perturb.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42perturb.c -o $(OBJ)42perturb.o

$(OBJ)42report.o    : $(SRC)42report.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(SRC)42report.c -o $(OBJ)42report.o

$(OBJ)42sensors.o   : $(SRC)42sensors.c $(INC)42.h $(INC)Ac.h $(INC)AcTypes.h
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

$(OBJ)gmseckit.o      : $(KITSRC)gmseckit.c $(KITINC)gmseckit.h
	$(CC) $(CFLAGS) -c $(KITSRC)gmseckit.c -o $(OBJ)gmseckit.o

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

$(OBJ)42CfdSlosh.o      : $(GSFCSRC)42CfdSlosh.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(GSFCSRC)42CfdSlosh.c -o $(OBJ)42CfdSlosh.o

$(OBJ)42fftb.o         : $(GSFCSRC)42fftb.c $(INC)42.h
	$(CC) $(CFLAGS) -c $(GSFCSRC)42fftb.c -o $(OBJ)42fftb.o

$(OBJ)AcApp.o          : $(SRC)AcApp.c $(INC)Ac.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(SRC)AcApp.c -o $(OBJ)AcApp.o

$(OBJ)SimWriteToFile.o  : $(IPCSRC)SimWriteToFile.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimWriteToFile.c -o $(OBJ)SimWriteToFile.o

$(OBJ)SimWriteToGmsec.o  : $(IPCSRC)SimWriteToGmsec.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimWriteToGmsec.c -o $(OBJ)SimWriteToGmsec.o

$(OBJ)SimWriteToSocket.o  : $(IPCSRC)SimWriteToSocket.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimWriteToSocket.c -o $(OBJ)SimWriteToSocket.o

$(OBJ)SimReadFromFile.o  : $(IPCSRC)SimReadFromFile.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimReadFromFile.c -o $(OBJ)SimReadFromFile.o

$(OBJ)SimReadFromGmsec.o  : $(IPCSRC)SimReadFromGmsec.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimReadFromGmsec.c -o $(OBJ)SimReadFromGmsec.o

$(OBJ)SimReadFromSocket.o  : $(IPCSRC)SimReadFromSocket.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)SimReadFromSocket.c -o $(OBJ)SimReadFromSocket.o

#$(OBJ)SimReadFromCmd.o  : $(IPCSRC)SimReadFromCmd.c $(INC)42.h $(INC)AcTypes.h
#	$(CC) $(CFLAGS) -c $(IPCSRC)SimReadFromCmd.c -o $(OBJ)SimReadFromCmd.o

$(OBJ)AppWriteToFile.o  : $(IPCSRC)AppWriteToFile.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppWriteToFile.c -o $(OBJ)AppWriteToFile.o

$(OBJ)AppWriteToGmsec.o  : $(IPCSRC)AppWriteToGmsec.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppWriteToGmsec.c -o $(OBJ)AppWriteToGmsec.o

$(OBJ)AppWriteToSocket.o  : $(IPCSRC)AppWriteToSocket.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppWriteToSocket.c -o $(OBJ)AppWriteToSocket.o

$(OBJ)AppReadFromFile.o  : $(IPCSRC)AppReadFromFile.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppReadFromFile.c -o $(OBJ)AppReadFromFile.o

$(OBJ)AppReadFromGmsec.o  : $(IPCSRC)AppReadFromGmsec.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppReadFromGmsec.c -o $(OBJ)AppReadFromGmsec.o

$(OBJ)AppReadFromSocket.o  : $(IPCSRC)AppReadFromSocket.c $(INC)42.h $(INC)AcTypes.h
	$(CC) $(CFLAGS) -c $(IPCSRC)AppReadFromSocket.c -o $(OBJ)AppReadFromSocket.o

$(OBJ)42nos3.o         : $(SRC)42nos3.c
	$(CC) $(CFLAGS) -c $(SRC)42nos3.c -o $(OBJ)42nos3.o

########################  Miscellaneous Rules  ############################
clean :
ifeq ($(42PLATFORM),_WIN32)
	del .\Object\*.o .\$(EXENAME) .\InOut\*.42
else ifeq ($(42PLATFORM),_WIN64)
	del .\Object\*.o .\$(EXENAME) .\InOut\*.42
else
	rm -f $(OBJ)*.o ./$(EXENAME) ./AcApp $(KITDIR)libkit.so $(INOUT)*.42 ./Standalone/*.42 ./Demo/*.42 ./Rx/*.42 ./Tx/*.42
endif
