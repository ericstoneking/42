HOW TO INSTALL WHICH WINDOWS
---------
We are going to use ubuntu bash on windows

Settings -> Update & security -> For developers, and activate "Developer mode"
Turn Windows Features on or off 
      C:\Windows\System32\OptionalFeatures.exe
Activate "Windows Subsystem for Linux (Beta)"
Wait for the download

Try to open the console and type "bash" if don't work restart computer...

REFERENCE:
`Ubuntu bash on windows <https://www.xataka.com/aplicaciones/asi-es-usar-la-consola-bash-de-ubuntu-en-windows-10/>`_

Download 42
Put 42 folder in C:\
or open a terminal:

.. code-block:: console

     & bash
     & git clone https://github.com/ericstoneking/42.git 
     & sudo apt update

Instal `Xming <https://sourceforge.net/projects/xming/>`_ on windows
Restart computer
Open terminal

.. code-block:: console

      & bash
      & sudo apt update
      & sudo apt-get install vim-gtk
      & sudo apt install gcc
      & DISPLAY=:0 gvim #check that opens a xming window
      & sudo apt-get install freeglut3-dev
      & cd 42
      & make


      


