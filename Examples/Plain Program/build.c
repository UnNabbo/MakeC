#define ENTRY_POINT Entry
#define QUIET_MODE false
#include "../../makec.h"

void Entry(){
	project Engine = ProjectCreate("Engine", COMPILER_MSVC, OUTPUT_LIBRARY);
	ProjectSetFilePath(&Engine, "src"); // EQUIVALENT OF DOING: Engine.FilePath = STR("src") 
	ProjectSetOutputPath(&Engine, "out");// EQUIVALENT OF DOING: Engine.OutputPath = STR("out") 
	
	ProjectAddFiles(&Engine, "*.cpp");
	
	ProjectAddCompilerFlags(&Engine, "/Zi /nologo");
	ProjectCompileAndWait(&Engine);
}