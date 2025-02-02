#define ENTRY_POINT Entry
#define QUIET_MODE true
#include "../../makec.h"

s32 Entry(){
	project * Projects = ProjectCreateFromFile("build.cfg");
	b32 Result = 0;
	for_each(Project, Projects){
		Result |= ProjectCompileAndWait(&Project);
	}
	return Result;
}
