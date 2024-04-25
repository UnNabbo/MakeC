#include "../../makec.h"

int main(void){
	project Project = NewProject("HelloMakeC", OUTPUT_KIND_EXE, COMPILER_KIND_MSVC);
	AddFileToProject(&Project, "main.c");
	
	int ErrorCode = 0;
	CompileProjectAndWait(&Project, &ErrorCode,  COMPILATION_SETTINGS_NONE);
	return 0;							
}