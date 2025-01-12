
void MakeCRecompile(char * ExeName){
	char * cPath = __FILE__;
	int i = 0;	for(i = strlen(cPath); i > 0 && cPath[i] != '\\'; i--){}
	string Path = StringAllocSubstr(cPath, i);
	string StrPath = StringFormat(STR("%\\build.exe.old"), Path);
	FileDelete(StrPath.Base);
	FileRename(ExeName, StrPath.Base);
	string Command = StringFormat(STR("cl /nologo /std:c++20 /O2 /EHsc /cgthreads8 %/build.c /link /OUT:%"), Path, STR(ExeName));
	if(ExecuteCommand(Command.Base)){
		FileRename(StrPath.Base, ExeName);
	}
	StringFree(&Command);
		
	for(i = strlen(ExeName); i > 0 && ExeName[i] != '.'; i--){}
	string Name = StringAllocSubstr(ExeName, i);
	string Obj = StringFormat(STR("%.obj"), Name);
		
	FileDelete(Obj.Base);
	StringFree(&Name);
	StringFree(&Obj);
	StringFree(&Path);
	StringFree(&StrPath);
}

#ifdef ENTRY_POINT

s32 ENTRY_POINT();

#define VALUE(string) #string
#define TO_LITERAL(string) VALUE(string)

int main(s32 Argc, char * Argv[]){
	Arguments.Literals = ArrayAlloc(string);
	ArgumentsLoad(Argc, Argv);
	s32 Result = ENTRY_POINT();
	if(ArgumentsSearch("--no_recompile") == ARGUMENT_NOT_FOUND){
		MakeCRecompile(Argv[0]);
	}
	return Result;
}
#endif
