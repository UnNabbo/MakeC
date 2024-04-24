#include "makec.h"

int main(void){
	
	project Project = NewProject("Test", OUTPUT_KIND_EXE, COMPILER_KIND_MSVC);
	SetProjectCompilationFlags(&Project, "/std:c++20 /W0 /EHsc /Z7 /external:W0");
	SetProjectPath(&Project, "test");
	SetProjectOutputPath(&Project, "out/test");
	//SetProjectLinkerFlags(&Project, "/incremental:no");
	AddFilesToProject(&Project, "*.c");
	SetProjectLibs(&Project, WIN32_LIBS);
	//AddProjectLibs(&Project, "");
	SetProjectLibrariesDirectories(&Project, "test");
	CopyAllFile("test", Project.OutputPath);
	
	int ErrorCode = 0;
	CompileProject(&Project, &ErrorCode,  COMPILATION_SETTINGS_NONE);
	WaitForProjectCompilation(&Project);
	
	system("pause > 0");
	return 0;							
}