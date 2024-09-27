#define ENTRY_POINT Entry
#define ENABLE_QUIET_MODE 
#include "../../makec.h"

void Entry(){
	project Engine = ProjectCreate("Engine", COMPILER_MSVC, OUTPUT_EXECUTABLE);
	ProjectSetFilePath(&Engine, "src"); // EQUIVALENT OF DOING: Engine.FilePath = STR("src") 
	ProjectSetOutputPath(&Engine, "out");// EQUIVALENT OF DOING: Engine.OutputPath = STR("out") 
	
	ProjectAddSourceFiles(&Engine, "main.c");
	
	ProjectAddCompilerFlags(&Engine, "/Zi /nologo");
	ProjectCompileAndWait(&Engine);
}