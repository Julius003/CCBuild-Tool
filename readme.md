# ccbuild
***
## Description
This is a tool for building small to medium sized c++
Projects that use multiple files.

## Usage
The tool can be used with the following command:
```bash
ccbuild ./myMainFile.cpp -o outputName
```

## How it works
The tool opens the main c++ file and looks 
for `#include "myHeader.h"` statements. It supports
header files with these extensions:
` .h  .hh  .hpp `.
When it has found a header file, it repeats the process recursively until 
all included files are found.
Then is begins a search and tries to find the right source files.
The tool does this by matching the header file name against variations
of it with the following extensions:
` .cpp  .cc  .C  .cxx  .cppm `
When it has found the source files, it executes the standard g++
build command with the appropirate arguments.

## Installation

Make sure you have g++ installed.

To install the tool, download the ccbuild executable or compile it yourself.
Then move it to a folder in your computer and create register the PATH
to it in your Environment Variables. 

Voila! You can now use the tool.
