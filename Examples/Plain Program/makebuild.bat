@echo off
cl /nologo /std:c++20 /W0 /EHsc /O2 /external:W0 /cgthreads8 build.c /link /OUT:build.exe

::gcc build.c -O3 -lstdc++ -o build.exe