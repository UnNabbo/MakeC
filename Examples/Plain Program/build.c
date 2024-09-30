#define ENTRY_POINT Entry
#define QUIET_MODE false
#include "../../makec.h"

void Entry(){
	project Engine = ProjectCreate("Engine", COMPILER_MSVC, OUTPUT_EXECUTABLE);
	ProjectAddFiles(&Engine, "*.cpp");
	ProjectCompileAndWait(&Engine);
}