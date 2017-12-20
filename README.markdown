HPL1 Engine Source Code
=======================

Yes, here it is at last the Engine that made the Penumbra Series.

Read through the TODO file for various known things that should be cleaned up / fixed.

Included are project files for Xcode, Visual Studio 2003 and Cmake (for Linux)

Contributing Code
-----------------
We encourage everyone to contribute code to this project, so just sign up for a github account, create a fork and hack away at the codebase. We will start an Open Source forum on the Frictional Games forums as a place to talk about changes and to submit patches from your forks.

License Information
-------------------
All code is under the GPL Version 3 license except for the "test" which are included under the ZLIB license. All of the assets are licensed under the Creative Commons Attribution Share-Alike 3.0 license except for the CG shaders which are under the ZLIB license.  Please read the COPYING and LICENSE-\* files for more information on terms of use.

How to compile the engine
-------------------------

You will need the following tools:
* GNU g++
* GNU Make
* cmake

You will also need to install the following dependencies:
* angelscript (version 2.7.1b, available at: http://www.angelcode.com/angelscript/downloads.html)
* OALWrapper (available at: https://github.com/FrictionalGames/OALWrapper)
* newton-dynamics (available at: https://github.com/MADEAPPS/newton-dynamics)
* TinyXml (apt install libtinyxml-dev)
* OpenAL (apt install libopenal-dev)
* SDL 1.2, SDL_ttf (apt install libsdl1.2-dev libsdl-ttf2.0-dev)
* libalut (apt install libalut-dev)

Create a directory which will be the root of your project.
```
mkdir penumbra
```

Then, git-clone HPL1Engine and OALWrapper under this directory:
```
git clone <HPL1_URL>       penumbra/HPL1Engine
git clone <OALWrapper_URL> penumbra/OALWrapper
```

