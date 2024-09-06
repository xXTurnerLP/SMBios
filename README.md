# About
Simple C++ SMBIOS parsing library for Windows without dependencies.  
Created for my own use, might be missing some features/tables from the SMBIOS specification

# How to build
You can get the library prebuilt in the [Releases](https://github.com/xXTurnerLP/SMBios/releases/tag/latest) section

1. You will need to install [premake5](https://premake.github.io/download) and put it in your PATH variable on Windows or place the .exe in the same folder as the project root
1. After you have installed premake5, run the `premake5_generate_projects.bat` script
1. A `build` folder should be created along with `SMBios.sln` file, open the `SMBios.sln` file in Visual Studio 2022
1. Choose your preset `Release` or `Debug` and build the solution (`Build -> Build Solution`)
1. In the `build` folder you can find your needed files

# How to use in C++
You can find a `test.cpp` file in the `example` folder, it contains all the info you need to use this library, if you still cant figure it out look at the source files (`SMBios.cpp` and `SMBios.hpp`)

# How to use in C#
In the `src` folder there is a class that can be used to interface with the library's DLL
