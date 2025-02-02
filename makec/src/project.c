static project ProjectCreate(char * Name, u32 Compiler, u32 Output){
	project Project;
	ZeroMemory(Project);
	
	Project.Name = Name;
	
	Project.OutputPath = ".";
	Project.Compiler = Compiler;
	Project.Output = Output;


	Project.SourceFiles = ArrayAlloc(string);
	Project.Libs = ArrayAlloc(string);
	Project.LibsDirs = ArrayAlloc(string);
	Project.IncludeDirs = ArrayAlloc(string);
	Project.CompilerFlags = ArrayAlloc(string);
	Project.LinkerFlags = ArrayAlloc(string);
	Project.Symbols = ArrayAlloc(string); 
	Project.Defines = ArrayAlloc(string);
	Project.Dependency = ArrayAlloc(string);
	
	return Project;
}

static inline void ProjectSetCompiler(project * Project, u32 Compiler){
	Project->Compiler = Compiler;
}

static inline void ProjectSetOutputType(project * Project, u32 Output){
	Project->Output = Output;
}

static inline void ProjectSetFilePath(project * Project, char * Path){
	Project->FilePath = Path;
}

static inline  void ProjectSetOutputPath(project * Project, char * Path){
	FolderCreate(Path);
	Project->OutputPath = Path;
}

static inline void ProjectAddFiles(project * Project, char * Paths){
	string * Files = ArrayAlloc(string);
	StringSemanticSplit(&Files, STR(Paths), ' ');
	
	for(int i = 0; i < ArraySize(Files); i++){	
		string Path = Files[i];
		b32 IsAPattern = false;
		for(int j = 0; j < Path.Size; j++){
			if(Path.Base[j] == '*'){
				IsAPattern = true;
			}
		}
		
		string FullPath;
		if(!IsAPattern){
			FullPath = (Project->FilePath) ? StringFormat(STR("%/%"), STR(Project->FilePath), STR(Path.Base)) : StringAlloc(Path.Base);
			ArrayAppend(&Project->SourceFiles, &FullPath);
		}else{
			FullPath = (Project->FilePath) ? StringFormat(STR("%/%"), STR(Project->FilePath), STR(Path.Base)) : StringAlloc(Path.Base);
			FileGLOB(&Project->SourceFiles, FullPath.Base);		
			StringFree(&FullPath);
		}
	}
	ArrayFree(Files);
}

static inline void ProjectResetFiles(project * Project){
	for_each(String, Project->SourceFiles){
		StringFree(&String);
	}
}

static inline void ProjectAddCompilerFlags(project * Project, char * Flags){
	StringSemanticSplitAlloc(&Project->CompilerFlags, StringCreate(Flags), ' ');
}

static inline void ProjectResetCompilerFlags(project * Project){
	for_each(String, Project->CompilerFlags){
		StringFree(&String);
	}
}

static inline void ProjectAddDependency(project * Project, char * Projects){
	StringSemanticSplitAlloc(&Project->Dependency, StringCreate(Projects), ' ');
}

static inline void ProjectResetDependency(project * Project){
	for_each(String, Project->Dependency){
		StringFree(&String);
	}
}

static inline void ProjectAddLinkerFlags(project * Project, char * Flags){
	StringSemanticSplitAlloc(&Project->LinkerFlags, StringCreate(Flags), ' ');
}

static inline void ProjectResetLinkerFlags(project * Project){
	for_each(String, Project->LinkerFlags){
		StringFree(&String);
	}
}

static inline void ProjectLinkLibs(project * Project,  char * Libs){
	StringSemanticSplitAlloc(&Project->Libs, StringCreate(Libs), ' ');
}

static inline void ProjectAddLibsDirs(project * Project, char * Dirs){
	StringSemanticSplitAlloc(&Project->LibsDirs, StringCreate(Dirs), ' ');
}

static inline void ProjectResetLibsDirs(project * Project){
	for_each(String, Project->LibsDirs){
		StringFree(&String);
	}
}

static inline void ProjectResetLibs(project * Project){
	for_each(String, Project->Libs){
		StringFree(&String);
	}
}

static inline void ProjectAddIncludeDirs(project * Project, char * Dirs){
	StringSemanticSplitAlloc(&Project->IncludeDirs, StringCreate(Dirs), ' ');
}

static inline void ProjectResetIncludeDirs(project * Project){
	for_each(String, Project->IncludeDirs){
		StringFree(&String);
	}
}

static inline void ProjectAddDefines(project * Project, char * Defines){
	StringSemanticSplitAlloc(&Project->Defines, StringCreate(Defines), ' ');
}

static inline void ProjectResetDefines(project * Project){
	for_each(String, Project->Defines){
		StringFree(&String);
	}
}

static inline void ProjectExportSymbols(project * Project, char * Symbols){
	StringSemanticSplitAlloc(&Project->Symbols, StringCreate(Symbols), ' ');
}

static inline void ProjectResetSymbols(project * Project){
	for_each(String, Project->Symbols){
		StringFree(&String);
	}
}

static inline void ProjectLink(project * LinkingProject, project * LinkedProject){
	string Lib = StringFormat(STR("%/%/%.lib"), STR(LinkedProject->OutputPath), STR(LinkedProject->Name), STR(LinkedProject->Name));
	ProjectLinkLibs(LinkingProject, Lib.Base);
	StringFree(&Lib);
}

static inline void ProjectWait(project * Project){
	if(Project->Thread){
		WaitForSingleObject(Project->Thread, INFINITE);
	}
}

static inline void ProjectLaunch(project * Project){
	string Command;
	if(Project->OutputPath){
		Command = StringFormat(STR("cd %/%/ & start %.exe"), STR(Project->OutputPath), STR(Project->Name), STR(Project->Name));
	}else{
		Command = StringFormat(STR("cd %/ start %.exe"), STR(Project->Name), STR(Project->Name));
	}
	system(Command.Base);
	StringFree(&Command);
}

static inline void ProjectLaunchWithArgs(project * Project, char * Args){
	string Command;
	if(Project->OutputPath){
		Command = StringFormat(STR("cd %/%/ & start %.exe %"), STR(Project->OutputPath), STR(Project->Name), STR(Project->Name), STR(Args));
	}else{
		Command = StringFormat(STR("cd %/ start %.exe %"), STR(Project->Name), STR(Project->Name), STR(Args));
	}
	system(Command.Base);
	StringFree(&Command);
}

static inline b32 ProjectIsRunning(project * Project){
	if(Project->Output != OUTPUT_EXECUTABLE){
		return 0;
	}
	
	string Process;
	if(Project->OutputPath){
		Process = StringFormat(STR("%/%.exe"), STR(Project->OutputPath), STR(Project->Name));
	}else{
		Process = StringFormat(STR("%.exe"), STR(Project->Name));
	}
	b32 Result = ProcessIsRunning(Process.Base);
	StringFree(&Process);
	return Result;
}


#if 0
// NOTE(Nabbo): Currently broken
static void ProjectCompile(project * Project, s32 * ErrorCode){
	compilation_data Data;
	Data.Project = MemReplicate(Project);
	Data.ErrorCode = ErrorCode;
	Data.MultiThreaded = true;
	
	Project->Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Compile, &Data, 0, NULL);
	if(!Project->Thread){
		// TODO(Nabbo): Assertion!
	}
}
#endif

static inline s32 ProjectCompileAndWait(project * Project){
	compilation_data Data;
	Data.Project = Project;
	s32 ErrorCode = 0;
	Data.ErrorCode = &ErrorCode;
	Data.MultiThreaded = false;
	
	Compile(Data);
	
	return ErrorCode;
}

static project * ProjectCreateFromFile(char * FilePath){
	token * Tokens = Tokenize(FilePath);
	
#if 0
	for_each(Token, Tokens){
		TokenPrint(Token);
	}
#endif
	
	project *Project = TokenParseIntoProject(&Tokens);
	
	ArrayFree(Tokens);

	
	return Project;
}
 
