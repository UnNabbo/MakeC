@echo off
cl /nologo /std:c++20 /W0 /EHsc /Zi /external:W0 /cgthreads8 build.c /link /OUT:build.exe
::gcc build.c -O3 -lstdc++ -o builder.exe
build.exe %*
