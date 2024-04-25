# MakeC
A simple C/C++ build system written in C that uses C as a project specification language

###  Usage
Using MakeC is really easy as it comes as a single include header that you can just copy in your project, include it in any C/C++ file, write your project specification in C/C++ and then compile that file with either the makebuild.bat provided in any of the examples or with any modern C/C++ compiler.
###  Writing a project specification
As a previusly stated MakeC uses C/C++ as a project specification language so that you do not have to learn an entirely new scripting language just to compile your C/C++ projects.
A quick example on how MakeC uses C/C++ as a project spefication language follows:
```cpp
project Project = NewProject("HelloMakeC", OUTPUT_KIND_EXE, COMPILER_KIND_MSVC);
AddFileToProject(&Project, "main.c");

int ErrorCode = 0;
CompileProjectAndWait(&Project, &ErrorCode,  COMPILATION_SETTINGS_NONE);					
```
MakeC would translate this code to the following compilation command
``
cl /nologo /FC /Fo:"./" /MD main.c /link /OUT:"HelloMakeC.exe"
``
More examples are available in the repository.