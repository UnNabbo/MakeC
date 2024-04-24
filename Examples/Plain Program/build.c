#include "../../makec.h"

int main(void){
	project Project = NewProject("HelloMakeC", OUTPUT_KIND_EXE, COMPILER_KIND_MSVC);
	SetProjectCompilationFlags(&Project, "/std:c++20");
	AddFileToProject(&Project, "main.c");
	
	int ErrorCode = 0;
	CompileProjectAndWait(&Project, &ErrorCode,  COMPILATION_SETTINGS_NONE);
	return 0;							
}