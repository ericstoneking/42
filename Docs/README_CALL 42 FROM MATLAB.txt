## PROCEDURE TO CALL 42 FROM MATLAB ##

1. Disable the 42's plots doing the following:
	- open the file "Makefile" located at the 42's root folder	
	- Comment line #27: "GUIFLAG = -D_ENABLE_GUI_"
	- Uncomment line #28: ""GUIFLAG ="
	- Save

2. Include in the PATH of the Windows (Environment variables):
	- C:\42ExternalSupport\ffmpeg\bin
	- C:\42ExternalSupport\glew\bin
	- C:\42ExternalSupport\freeglut\bin
	- C:\MingGW\msys\1.0\bin

3. Compile (make) the 42 file from "msys" platform.

4. In the Windows Command or Matlab Command, type: 
	>> system('bash -c "42 FileName"')