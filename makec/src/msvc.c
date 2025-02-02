static string MSVCGenerateCompilationCommand(project * Project){
	string OutputExtension;
	switch(Project->Output){
		
		case OUTPUT_EXECUTABLE:{
			OutputExtension = STR(".exe");
		}break;
		
		case OUTPUT_DYNAMIC_LIBRARY:{
			OutputExtension = STR(".dll");
		
			ProjectAddCompilerFlags(Project, "/LD");
			ProjectAddLinkerFlags(Project, "/OPT:REF");
		}break;
		
		case OUTPUT_LIBRARY:{
			OutputExtension = STR(".lib");
			
			ProjectAddCompilerFlags(Project, "/c");
		}break;
	}

	
	string TUnitPath;
	if(Project->OutputPath){
		TUnitPath = StringFormat(STR("%/%/"), STR(Project->OutputPath), STR(Project->Name));
	}else{
		TUnitPath = StringFormat(STR("%/"), STR(Project->Name));
	}
	FolderCreate(TUnitPath.Base);
	
	string SourceFiles = StringListJoin(&Project->SourceFiles, ' ');
	
	string CompilerFlags = StringListJoin(&Project->CompilerFlags, ' ');
	
	string* RawDefines = ArrayCopy(Project->Defines);
	for(int i = 0; i < ArraySize(RawDefines); i++){
		RawDefines[i] = StringFormat(STR("/D%"), Project->Defines[i]);
	}
	string Defines = StringListJoin(&RawDefines, ' ');
	ArrayFree(RawDefines);
	
	string* RawIncludeDirs = ArrayCopy(Project->IncludeDirs);
	for(int i = 0; i < ArraySize(RawIncludeDirs); i++){
		RawIncludeDirs[i] = StringFormat(STR("/external:I%"), Project->IncludeDirs[i]);
	}
	string IncludeDirs = StringListJoin(&RawIncludeDirs, ' ');
	ArrayFree(RawIncludeDirs);
	
	string Libs = StringListJoin(&Project->Libs, ' ');
	string LinkerFlags = StringListJoin(&Project->LinkerFlags, ' ');
	
	string* RawSymbols = ArrayCopy(Project->Symbols);
	for(int i = 0; i < ArraySize(RawSymbols); i++){
		RawSymbols[i] = StringFormat(STR("/EXPORT:%"), Project->Symbols[i]);
	}
	string Symbols = StringListJoin(&RawSymbols, ' ');
	ArrayFree(RawSymbols);
	
	string* RawLibsDirs = ArrayCopy(Project->LibsDirs);
	for(int i = 0; i < ArraySize(RawLibsDirs); i++){
		RawLibsDirs[i] = StringFormat(STR("/LIBPATH:%"), Project->LibsDirs[i]);
	}
	string LibsDirs = StringListJoin(&RawLibsDirs, ' ');
	ArrayFree(RawLibsDirs);
	
	string CompilationCommand;
	
	if(Project->Output != OUTPUT_LIBRARY){
		CompilationCommand = StringFormat(STR("cl /nologo /FC /Fo:\"%\" /Fe:\"%\" /Fd:\"%\" % % % % /link % % % % /OUT:\"%/%%\""), TUnitPath, TUnitPath ,TUnitPath, CompilerFlags, Defines, SourceFiles, IncludeDirs, LinkerFlags, LibsDirs, Libs, Symbols, TUnitPath, STR(Project->Name), OutputExtension);
	}else{
		CompilationCommand = StringFormat(STR("cl /nologo /FC /Fo:\"%\" /Fe:\"%\" /Fd:\"%\" % % % %"), TUnitPath, TUnitPath ,TUnitPath, CompilerFlags, Defines, SourceFiles, IncludeDirs);
	}
	
	StringFree(&TUnitPath);
	
	StringFree(&SourceFiles);
	
	StringFree(&CompilerFlags);
	StringFree(&Defines);
	StringFree(&IncludeDirs);
	
	StringFree(&Libs);
	StringFree(&LibsDirs);
	StringFree(&LinkerFlags);
	StringFree(&Symbols);
	
	return CompilationCommand;
}

static string MSVCGenerateLinkCommand(project * Project){
	string Libs = StringListJoin(&Project->Libs, ' ');
	string LinkerFlags = StringListJoin(&Project->LinkerFlags, ' ');
	
	string* RawSymbols = ArrayCopy(Project->Symbols);
	for(int i = 0; i < ArraySize(RawSymbols); i++){
		RawSymbols[i] = StringFormat(STR("/EXPORT:%"), Project->Symbols[i]);
	}
	string Symbols = StringListJoin(&RawSymbols, ' ');
	for_each(String, RawSymbols){
		StringFree(&String);
	}
	ArrayFree(RawSymbols);
	
	string* RawLibsDirs = ArrayCopy(Project->LibsDirs);
	for(int i = 0; i < ArraySize(RawLibsDirs); i++){
		RawLibsDirs[i] = StringFormat(STR("/LIBPATH:%"), Project->LibsDirs[i]);
	}
	string LibsDirs = StringListJoin(&RawLibsDirs, ' ');
	for_each(String, RawLibsDirs){
		StringFree(&String);
	}
	ArrayFree(RawLibsDirs);
	
	
 	string TsPattern = StringFormat(STR("%/%/*.obj"), STR(Project->OutputPath), STR(Project->Name));
	string * RawTranslationUnits = ArrayAlloc(string);
	FileGLOB(&RawTranslationUnits, TsPattern.Base);
	StringFree(&TsPattern);
	string TranslationUnits = StringListJoin(&RawTranslationUnits, ' '); 
	for_each(String, RawTranslationUnits){
		StringFree(&String);
	}
	ArrayFree(RawTranslationUnits);
 	
	string LinkCommand = StringFormat(STR("lib % % % % /OUT:\"%/%/%.lib\""), TranslationUnits, LibsDirs, Libs, Symbols, STR(Project->OutputPath),  STR(Project->Name), STR(Project->Name));
	
	StringFree(&Libs);
	StringFree(&LibsDirs);
	StringFree(&LinkerFlags);
	StringFree(&Symbols);
	StringFree(&TranslationUnits);
	return LinkCommand;
}

static void Compile(compilation_data Data){
	project * Project = Data.Project;


	string Command = MSVCGenerateCompilationCommand(Project);
	
#if !QUIET_MODE	
	string StartTime = TimeGetCurrent();
	StringPrintf(STR("\nCompilation of % started at %"), STR(Project->Name), StartTime);
	StringFree(&StartTime);

	u64 Ticks = QueryPerformanceGetCounter();
#endif
	//printf("Command: %s\n", Command.Base);
	s32 ErrorCode = ExecuteCommand(Command.Base);
	*Data.ErrorCode |= ErrorCode;
	
#if !QUIET_MODE
	Ticks = QueryPerformanceGetCounter() - Ticks;
	Printf(STR("\n%\n"), Command);
	string EndTime = TimeGetCurrent();
	
	f64 Seconds = (f64)Ticks / QueryPerfonceGetFreq();
	
	string Result = (ErrorCode) ? STR("exited") : STR("finished");
	string Ms = F64ToString(Seconds * 1000, 3);
	string Sc = F64ToString(Seconds, 3);
	StringPrintf(STR("Compilation of % % at % after %ms(%s)\n"), STR(Project->Name), Result, EndTime, Ms, Sc);
	StringFree(&EndTime);
	StringFree(&Ms);
	StringFree(&Sc);
#endif
	
	
	if(Project->Output == OUTPUT_LIBRARY){
		string LinkCommand = MSVCGenerateLinkCommand(Project);
		ErrorCode = ExecuteCommand(LinkCommand.Base);
		*Data.ErrorCode |= ErrorCode;
	} 
	
	if(Data.MultiThreaded){
		MemFree(Project);
		ExitThread(0);
	}
}
