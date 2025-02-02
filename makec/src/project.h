enum compiler{
	COMPILER_MSVC,
	//COMPILER_,
	
	COMPILER_COUNT,
};

enum output{
	OUTPUT_EXECUTABLE,
	OUTPUT_DYNAMIC_LIBRARY,
	OUTPUT_LIBRARY,
	
	OUTPUT_COUNT,
};

typedef struct{
	char * Name;
	
	char * FilePath;
	char * OutputPath;
	
	string * SourceFiles; //DONE
	
	string* Libs; //DONE
	string* LibsDirs;  //DONE
	
	string* IncludeDirs;
	
	string* CompilerFlags; //DONE
	string* LinkerFlags; //DONE
	string* Symbols;  //DONE
	string* Defines;  //DONE
	string* Dependency;  //DONE
	
	u32 Compiler;
	u32 Output;
	
	void* Thread;
}project;

typedef struct {
	project * Project;
	s32 * ErrorCode;
	b32 MultiThreaded;
}compilation_data;

project ProjectCreate(char * Name, u32 Compiler, u32 Output);
void ProjectSetCompiler(project * Project, u32 Compiler);
void ProjectSetOutputType(project * Project, u32 Output);
void ProjectSetFilePath(project * Project, char * Path);
void ProjectSetOutputPath(project * Project, char * Path);
void ProjectAddFiles(project * Project, char * Files);
void ProjectResetFiles(project * Project);
void ProjectAddCompilerFlags(project * Project, char * Flags);
void ProjectResetCompilerFlags(project * Project);
void ProjectAddLinkerFlags(project * Project, char * Flags);
void ProjectResetLinkerFlags(project * Project);
void ProjectLinkLibs(project * Project, char * Libs);
void ProjectAddLibsDirs(project * Project, char * Dirs);
void ProjectResetLibsDirs(project * Project);
void ProjectResetLibs(project * Project);
void ProjectAddIncludeDirs(project * Project, char * Dirs);
void ProjectResetIncludeDirs(project * Project);
void ProjectAddDefines(project * Project, char * Defines);
void ProjectResetDefines(project * Project);
void ProjectExportSymbols(project * Project, char * Symbols);
void ProjectResetSymbols(project * Project);
void ProjectLink(project * LinkingProject, project * LinkedProject);
void ProjectWait(project * Project);
void ProjectLaunch(project * Project);
b32 ProjectIsRunning(project * Project);
void ProjectCompile(project * Project, s32 * ErrorCode);
s32 ProjectCompileAndWait(project * Project);
project * ProjectCreateFromFile(char * FilePath);                
 
