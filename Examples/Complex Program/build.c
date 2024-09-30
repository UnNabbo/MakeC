#define ENTRY_POINT Entry
#define QUIET_MODE false
#include "../../makec.h"

typedef enum {
	DEBUG,
	RELEASE,
} custom_compile_flag;

typedef enum {
	ENGINE  = 1,
	GAME = 2,
} custom_target_flag;

void ProjectSetup(project * Project, u32 Flags){
	ProjectSetFilePath(Project, Project->Name);
	ProjectSetOutputPath(Project, "out");
	
	ProjectAddCompilerFlags(Project, "/std:c++20 /W0 /EHsc /external:W0 /DYNAMICBASE");
	switch(Flags){
		case DEBUG:{
			ProjectAddCompilerFlags(Project, "/Zi");
		}break;
		
		case RELEASE:{
			ProjectAddCompilerFlags(Project, "/O2 /GL");
		}break;
	}
	
	ProjectAddIncludeDirs(Project, "Engine/src ./");

	ProjectAddFiles(Project, "src/*.cpp");
}

void Entry(){
	project Engine = ProjectCreate("Engine", COMPILER_MSVC, OUTPUT_LIBRARY);
	
	project Game = ProjectCreate("Game", COMPILER_MSVC, OUTPUT_EXECUTABLE);
	
	u32 Flags =(ArgumentsSearch("release")) ? RELEASE : DEBUG;
	
	u32 Target = GAME | ENGINE;
	switch(ArgumentsSearch("engine game")){
		case 0:{
			Target = ENGINE;
		}break;
		
		case 1:{
			Target = GAME;
		}break;
	}
	
	ProjectSetup(&Engine, Flags);
	ProjectSetup(&Game, Flags);
	ProjectLink(&Game, &Engine);
	
	if(Target & ENGINE){
		ProjectCompileAndWait(&Engine);
	}
	
	if(Target & GAME){
		ProjectCompileAndWait(&Game);
	}
}