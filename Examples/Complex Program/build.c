#include "../../makec.h"

void SetupProject(project * Project){
	SetProjectPath(Project, Project->Name);
	SetProjectOutputPath(Project, "out");
	
	//SetProjectCompilationFlags(Project, "/std:c++20 /W0 /EHsc /external:W0 /DYNAMICBASE");
	
	SetProjectIncludeDirectories(Project, "./");
	SetProjectLibrariesDirectories(Project, "out/");
	SetProjectIncludeDirectories(Project, "Engine/src");
	//SetProjectLibs(Project, WIN32_LIBS);
	
	AddFilesToProject(Project, "src/*.cpp");
}

int main(int argc, char *argv[]){
	project Engine = NewProject("Engine", OUTPUT_KIND_STATIC_LIBRARY, COMPILER_KIND_CLANG);
	project Game = NewProject("Game", OUTPUT_KIND_EXECUTABLE, COMPILER_KIND_CLANG);
	
	u32 CompilationSettings = COMPILATION_SETTINGS_MULTI_THREADED;
	
	u32 Bitfield = SearchFlagsInArgv(argc, argv, "Release");
	
	if(Bitfield & BIT_INDEX_0){
		CompilationSettings |= COMPILATION_SETTINGS_OPTIMIZED;
	}else{
		CompilationSettings |= COMPILATION_SETTINGS_DEBUG;
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
		CompileProjectAndWait(&Engine, &EngineErrorCode, CompilationSettings);
	}
	
	if(BuildFlag & BIT(1)){
		SetupProject(&Game);
		AddProjectLibs(&Game, "Engine");
		//AddProjectCompilationFlags(&Game, "/MD");
		CompileProjectAndWait(&Game, &GameErrorCode, CompilationSettings);
	}
	
	return 0;							
}