#**************  These lines thanks to http://www.sunandblackcat.com/tipFullView.php%3Fl%3Deng%26topicid%3D1 **************
gcc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude -DGLEW_BUILD -o src/glew.o -c src/glew.c

gcc -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32

#  If you get a build error like this:  multiple definition of `DllMainCRTStartup@12', consider adding the option -nostdlib to the link command.
#  See this link for a caution, though:
#  http://stackoverflow.com/questions/38673228/multiple-definition-of-dllmaincrtstartup12-while-building-glew-on-windows-wit

# This line has the -nostdlib option in it, for your convenience
#gcc -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a  -nostdlib -o lib/glew32.dll src/glew.o -L/mingw/lib -lglu32 -lopengl32 -lgdi32 -luser32 -lkernel32

ar cr lib/libglew32.a src/glew.o
#**************************************************************************************************************************
