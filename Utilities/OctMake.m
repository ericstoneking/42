% Octave-style m-file to make 42 into an .oct file, callable from Octave

mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/dcmkit.c -o ./Object/dcmkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/envkit.c -o ./Object/envkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/fswkit.c -o ./Object/fswkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/geomkit.c -o ./Object/geomkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/iokit.c -o ./Object/iokit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/mathkit.c -o ./Object/mathkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/msis86kit.c -o ./Object/msis86kit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/orbkit.c -o ./Object/orbkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/sigkit.c -o ./Object/sigkit.o
mkoctfile -I./Include/ -I./Kit/Include/  -c ./Kit/Source/timekit.c -o ./Object/timekit.o

mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42exec.c -o ./Object/42exec.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42actuators.c -o ./Object/42actuators.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42cmd.c -o ./Object/42cmd.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42dynam.c -o ./Object/42dynam.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42environs.c -o ./Object/42environs.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42ephem.c -o ./Object/42ephem.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42fsw.c -o ./Object/42fsw.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42init.c -o ./Object/42init.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42perturb.c -o ./Object/42perturb.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42report.c -o ./Object/42report.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/42sensors.c -o ./Object/42sensors.o
mkoctfile -I./Include/ -I./Kit/Include/ -c ./Source/forte.cpp -o ./Object/forte.o

mkoctfile -o forte.oct ./Object/dcmkit.o ./Object/envkit.o ./Object/fswkit.o ./Object/geomkit.o ./Object/iokit.o ./Object/mathkit.o ./Object/msis86kit.o ./Object/orbkit.o ./Object/sigkit.o ./Object/timekit.o ./Object/42exec.o ./Object/42actuators.o ./Object/42cmd.o ./Object/42dynam.o ./Object/42environs.o ./Object/42ephem.o ./Object/42fsw.o ./Object/42init.o ./Object/42perturb.o ./Object/42report.o ./Object/42sensors.o ./Object/forte.o

