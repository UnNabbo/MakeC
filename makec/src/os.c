#if defined(__clang__)
#define PipeOpen popen	
#define PipeClose pclose
#elif defined(__GNUC__) || defined(__GNUG__)
#define PipeOpen popen	
#define PipeClose pclose
#elif defined(_MSC_VER)
#define PipeOpen _popen	
#define PipeClose _pclose
#endif

static s32 ExecuteCommand(char * Command){
	FILE* Console = PipeOpen(Command, "r");
	if (!Console) {
        printf("Failed to open pipe\n");
    }
	char PipeBuffer[1024];
	ZeroMemory(PipeBuffer);
	
	char Output[4096 * 16];
	ZeroMemory(Output);
	s32 Offset = 0;
	
	s32 ErrorCode = 0;

	while (fgets(PipeBuffer, sizeof(PipeBuffer), Console) != 0) {
		int IsError = strstr(PipeBuffer, "error") != NULL;
		int IsWarning = strstr(PipeBuffer, "warning") != NULL;
		int IsNote = strstr(PipeBuffer, "note") != NULL;
		if (IsError | IsWarning | IsNote) {
			s32 Len = strlen(PipeBuffer);
			MemCopy(Output + Offset, PipeBuffer, Len);
			Offset += Len;
			ErrorCode = 1;
		}
	}
	
	if(ErrorCode){
		printf("%s", Output);
	}
	PipeClose(Console);
	return ErrorCode;
}

#ifdef _WIN32

static void FolderCreate(const char * Path){
	string Command = StringFormat(STR(">nul 2>&1 pushd \"%\" &&(popd & >nul 2>&1 mkdir \"%\")|| >nul 2>&1 mkdir \"%\""), STR(Path),  STR(Path),  STR(Path));
	ExecuteCommand(Command.Base);
	StringFree(&Command);
}

static void FileDelete(const char * Path){
	string Command = StringFormat(STR("del /F /Q % > NUL"), STR(Path));
	ExecuteCommand(Command.Base);
	StringFree(&Command);
}


static void FileRename(const char * Path, const char * NewPath){
	string Command = StringFormat(STR("move \"%\" \"%\""), STR(Path),  STR(NewPath));
	ExecuteCommand(Command.Base);
	StringFree(&Command);
}


static void FilesCopyAll(const char * Source, const char * Destination){
	string Command = StringFormat(STR("robocopy \"%\" \"%\""), STR(Source), STR(Destination));
	ExecuteCommand(Command.Base);
	StringFree(&Command);
}

static void FilesCopyAllMatching(const char * Source, const char * Destination, const char * Pattern){
	string Command = StringFormat(STR("xcopy /s/y \"%\\%\" \"%\""), STR(Source), STR(Pattern), STR(Destination));
	ExecuteCommand(Command.Base);
	StringFree(&Command);
}

static void FileGLOB(string ** List, const char* Pattern){
	s32 InitialSize = ArraySize(*List);
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(Pattern, &fd);
	
	s32 PathSize = 0;
	while(Pattern[PathSize] != '*' && Pattern[PathSize++]){}
	
	int Amount = 0;
	if(hFind != INVALID_HANDLE_VALUE) { 
		do { 
			if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
				string FileName = (PathSize) ? StringFormat(STR("%/%"),StringCreateWithSize(Pattern, PathSize - 1), STR(fd.cFileName)) : StringAlloc(fd.cFileName);
				b32 ShouldAdd = true;
				for(int i = 0; i < InitialSize; i++){
					if(StringCompare(FileName, (*List)[i])){
						ShouldAdd = false;
					}
				}
				if(ShouldAdd) ArrayAppend(List, &FileName);
			}
			
		}while(FindNextFile(hFind, &fd)); 
		FindClose(hFind); 
	}
}

static void FileRecursiveGLOB(string ** List,  char* Pattern){
	s32 InitialSize = ArraySize(*List);
	
	s32 PathSize = 0;
	s32 PatternSize = CStringLenght(Pattern);
	while(Pattern[PathSize] != '*' && Pattern[PathSize++]){}
	WIN32_FIND_DATA Data;
	string WildCard = StringAllocSubstr(Pattern + PathSize, PatternSize - PathSize);
	string Path = StringFormat(STR("%/*"),StringCreateWithSize(Pattern, PathSize - 1));
	
    HANDLE hFindFile = FindFirstFile(Path.Base, &Data);
    
    int Worked = 1;

    if (hFindFile != INVALID_HANDLE_VALUE) {
		 do {
			if ((Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(Data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
				if (!CStringCompare(Data.cFileName, ".") && !CStringCompare(Data.cFileName, "..") ) {
					string RecurisvePattern = (PathSize) ? StringFormat(STR("%/%/%"), StringCreateWithSize(Pattern, PathSize - 1), STR(Data.cFileName), WildCard) : StringFormat(STR("%/%"), Data.cFileName, WildCard);
					FileRecursiveGLOB(List, RecurisvePattern.Base);
					StringFree(&RecurisvePattern);
				}

			}else{
				s32 Offset = 0;
				while(Data.cFileName[Offset] != '.' && Data.cFileName[Offset++]){}
				if (CStringCompare(Data.cFileName + Offset, WildCard.Base + 1)) { 
					string FileName = (PathSize) ? StringFormat(STR("%/%"),StringCreateWithSize(Pattern, PathSize - 1), STR(Data.cFileName)) : StringAlloc(Data.cFileName);
					b32 ShouldAdd = true;
					for(int i = 0; i < InitialSize; i++){
						if(StringCompare(FileName, (*List)[i])){
							ShouldAdd = false;
						}
					}
					if(ShouldAdd) ArrayAppend(List, &FileName);
				}
			}
		 }while(FindNextFile(hFindFile, &Data));
    }
	StringFree(&WildCard);
	StringFree(&Path);
}

static char * EnviromentVariableGet(const char * VarName){
	char Buff[64];
	memset(Buff, 0, 64);
	GetEnvironmentVariable(VarName, Buff, 64);
	return CStringAlloc(Buff);
}

static b32 ProcessIsRunning(const char *ProcessName)
{
    b32 exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
	
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
		if (!strcmp(entry.szExeFile, ProcessName))
		exists = true;
	
    CloseHandle(snapshot);
    return exists;
}

static string TimeGetCurrent(){
	time_t RawTimeStart;
	time_t RawTimeEnd;
	
	time(&RawTimeStart);
	struct tm* StartTime = localtime(&RawTimeStart);
	char Buffer[128];
	ZeroMemory(Buffer);
	sprintf(Buffer, "%02d:%02d:%02d", StartTime->tm_hour, StartTime->tm_min, StartTime->tm_sec);
	
	return StringAlloc(Buffer);
	
}

static inline s64 QueryPerfonceGetFreq(){
	s64 freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	return freq;
}

static inline s64 QueryPerformanceGetCounter() {
	s64 value;
	QueryPerformanceCounter((LARGE_INTEGER*)&value);
	return value;
}

#else
#error Makec currently only supports windows
#endif
