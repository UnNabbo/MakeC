static command_line_args Arguments;

static void ArgumentsLoad(s32 Argc, char * Argv[]){
	for(int i = 1; i < Argc; i++){
		string Entry = STR(Argv[i]);
		ArrayAppend(&Arguments.Literals, &Entry);
	}
}


static s32 ArgumentsSearch(const char * Args){
	string* ArgsList = ArrayAlloc(string);
	StringSplit(&ArgsList, STR(Args), ' ');
	
	s32 Result = -1;
	
	for(int i = 0; i < ArraySize(Arguments.Literals); i++){
		for(int j = 0; j < ArraySize(ArgsList); j++){
			if(StringCompare(Arguments.Literals[i], ArgsList[j])){
				Result = j;
			}
		}
	}
	
	ArrayFree(ArgsList);
	return Result;
}
