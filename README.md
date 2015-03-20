# 42
Simulation for spacecraft attitude control system analysis and design 

Description

42 is a comprehensive general-purpose simulation of spacecraft attitude and orbit dynamics. Its primary purpose is to support design and validation of attitude control systems, from concept studies through integration and test. 42 accurately models multi-body spacecraft attitude dynamics (with rigid and/or flexible bodies), and both two-body and three-body orbital flight regimes, modelling environments from low Earth orbit to throughout the solar system. 42 simulates multiple spacecraft concurrently, facilitating studies of rendezvous, proximity operations, and precision formation flying. It also features visualization of spacecraft attitude.

Welcome to 42, the mostly harmless spacecraft simulation tool.

See the overview, "42 Overview.pdf", in the Docs folder.

If you're installing on Windows, see the file "Install-msys.txt" in the Docs folder.

Edit your Makefile to set your PLATFORM.

The default folder for inputs and outputs is "InOut".  Look there for sample input files.  "Inp_Sim.txt" is the top-level input file.

The input/output folder may be changed for a sim run by running 42 with an argument.  For example, enter this at the command prompt:
42 Demo

The VisCProj and XcodeProj folders are provided as starting points.  I haven't kept them up to date as 42 has evolved.  The Makefile is current.

There is a Prox Ops window that I had to leave out of the open source distribution for the present.  That's what the PROXOPSFLAG in the Makefile is about.  Check back in a couple months.
