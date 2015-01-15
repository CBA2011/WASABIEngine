# WASABIEngine
## CMake compilation
### Prerequisites
- CMake >= Version 2.8.11
- Qt5

### Compilation Process
To compile the WASABIEngine with cmake navigate a terminal instance to the root folder of your local copy of this repository and execute the following commands:

	mkdir build
	cd build/
	cmake ..
	make

## Original README.txt
Freiburg, 23rd of September, 2011
UPDATED 22nd of October, 2013

[W]ASABI [A]ffect [S]imulation [A]rchitecture for [B]elievable [I]nteractivity
Version 1.0
Copyright (C) 2011 Christian Becker-Asano.
All rights reserved.

Contact: Christian Becker-Asano (christian (at) becker-asano (dot) de)

1. Compiling
This library can be compiled as shared library (dll) using "Qt Creator" version 2.8.1 (MSVC 2010, Qt5.1.1)
under MS Windows 7. It is, however, not depending on Qt in any way.
To compile the library yourself it might be easiest to download the free "Qt SDK" from www.qt-project.org and then
open the WASABIEngine.pro file.

2. Documentation
I used doxygen to compile an html documentation from the source code. It can be found in
"WASABIEngine\doxygen\html".

3. Additional information
To get additional information about WASABI, reading my PhD thesis might be helpful.
@phdthesis{
  author = {Becker-Asano, Christian},
  title = {WASABI: Affect Simulation for Agents with Believable Interactivity},
  school = {Faculty of Technology, University of Bielefeld},
  year = {2008},
  note = {IOS Press (DISKI 319)},
  url = {http://www.becker-asano.de/Becker-Asano_WASABI_Thesis.pdf}
}

Anyway, please feel free to contact me.

Best,
CBA
