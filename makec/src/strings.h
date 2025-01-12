typedef struct{
	char * Base;
	s32 Size;
} string;

#define STR(str) StringCreate(str)
#define C_STR(str) str.Base

s32 CStringLenght(const char * String);
b32 CStringCompareN(const char * String1, const char* String2, s32 Len);
b32 CStringCompare(char * Str1, char * Str2);
char * CStringAlloc(char * String);
string StringCreate(const char * Text);
string StringCreateWithSize(const char * Text, s32 Size);
string StringAlloc(char * Text);
string StringAllocSubstr(char * Text, s32 Size);
void StringFree(string * String);
b32 StringCompare(string Str1, string Str2);
string StringListJoin(string** List, char Token);
void StringSplit(string** List, string String, char Token);
void StringSemanticSplit(string** List, string String, char Token);
void StringSemanticSplitAlloc(string** List, string String, char Token); 
string StringFormat(string Format, ...);
