@echo off
cl /nologo /std:c++20 /O2 /W0 /EHsc /external:W0 /cgthreads8 build.c /link /OUT:build.exe



::gcc build.c -O3 -lstdc++ -o build.exe
