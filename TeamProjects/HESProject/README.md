HES Project
===========
Software for solving and visualization heat equation.

How to build
-----------------
**Requirements**

Utilities needed to build HES Project:
* [Python 2.7](https://www.python.org/) (3.x.x not supported).
* [Node.js](http://nodejs.org/) (install version 0.11.13).
* [nw-gyp](https://github.com/rogerwang/nw-gyp) (to install folow instructions on project page).
* Microsoft Visual Studio 2010/2012/2013 on Windows.
* GCC, make on Linux.

**Instruction**

Folow instructions below. If you have installed several versions of Python on Windows execute `set PYTHON=<path to python.exe 2.7 version>`. Run configuration script `python configure.py install`. Configuration script supports folowing options:
* `install` - download need node-webkit version, rebuild HESProject.
* `installnw` - download need node-webkit version.
* `rebuild` - rebuild HESProject.
* `build`- compile and link only changed files.
* `clear` - remove generated files in build directory.

How to run application
----------------------
Start `nw` in `~/TeamProjects/ HESProject/UI/`.

You can also manual install node-webkit. Download node-webkit v0.10.5 build [here](https://github.com/rogerwang/node-webkit) and unpack to `HESProject/UI/`.
