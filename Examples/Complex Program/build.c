#include "../../makec.h"

void SetupProject(project * Project){
	SetProjectPath(Project, Project->Name);
	SetProjectOutputPath(Project, "out");
	
	SetProjectCompilationFlags(Project, "/std:c++20 /W0 /EHsc /Z7 /external:W0 /DYNAMICBASE");
	
	SetProjectIncludeDirectories(Project, "./");
	SetProjectLinkerFlags(Project, "/INCREMENTAL:NO");
	SetProjectLibrariesDirectories(Project, "out");
	SetProjectIncludeDirectories(Project, "Engine/src");
	SetProjectLibs(Project, WIN32_LIBS);
	
	AddFilesToProject(Project, "src/*.cpp");
}

int main(int argc, char *argv[]){
	project Engine = NewProject("Engine", OUTPUT_KIND_LIB, COMPILER_KIND_MSVC);
	project Game = NewProject("Game", OUTPUT_KIND_EXE, COMPILER_KIND_MSVC);
	
	u32 Bitfield = SearchFlagsInArgv(argc, argv, "Release");
	
	if(Bitfield & BIT_INDEX_0){
		AddProjectCompilationFlags(&Engine, "/O2");
		AddProjectCompilationFlags(&Game, "/O2");
	}else{
		AddProjectDefines(&Engine, "DEBUG");
		AddProjectDefines(&Game, "DEBUG");
	}
	
	u32 BuildTarget = AreBuildTargetsInArgv(argc, argv, "engine game");
	u32 BuildFlag = 0;
	switch(BuildTarget){
		case BIT_INDEX_0:{
			BuildFlag |= BIT(0);
		}break;
		
		case BIT_INDEX_1:{
			BuildFlag |= BIT(1);
		}break;
		
		default:{
			BuildFlag |= BIT(0) | BIT(1);			
		}break;
	}
	
	int EngineErrorCode = 0;
	int GameErrorCode = 0;
	
	if(BuildFlag & BIT(0)){
		SetupProject(&Engine);
		CompileProjectAndWait(&Engine, &EngineErrorCode, COMPILATION_SETTINGS_MULTI_THREADED);
	}
	
	if(BuildFlag & BIT(1)){
		SetupProject(&Game);
		AddProjectLibs(&Game, "Engine.lib");
		
		CompileProjectAndWait(&Game, &GameErrorCode, COMPILATION_SETTINGS_MULTI_THREADED);
	}
	
	if(!EngineErrorCode && ! GameErrorCode && BuildFlag & BIT(1)){
		RunProject(&Game);
	}
	
	return 0;							
}