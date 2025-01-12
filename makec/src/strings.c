inline s32 CStringLenght(const char * String){
	int i = 0;
	while(String[i]){
		i++;
	}
	return i;
}


b32 CStringCompareN(const char * String1, const char* String2, s32 Len){
    if(Len == 1){
        return String1[0] == String2[0];
    }
    if(CStringLenght(String1) < Len) return false;
    if(CStringLenght(String2) < Len) return false;
    
    b32 Result = true;
    for(int i = 0; i < Len; i++){
        Result &= String1[i] == String2[i];
    }
    return Result;
}

static b32 CStringCompare(char * Str1, char * Str2){
	if(CStringLenght(Str1) != CStringLenght(Str2)){
		return false;	
	}
	for(int i = 0; i < CStringLenght(Str1); i++){
		if(Str1[i] != Str2[i]){
			return false;
		}
	}
	return true;
}



inline char * CStringAlloc(char * String){
	s32 Size = CStringLenght(String);
	char * NewString = MemAlloc(Size);
	MemCopy(NewString, String, Size);
	return NewString;
}


inline string StringCreate(const char * Text){
	string String;
	String.Base = (char *)Text;
	String.Size = (Text) ? CStringLenght(Text) : 0;
	return String;
}

inline string StringCreateWithSize(const char * Text, s32 Size){
	string String;
	String.Base = (char *)Text;
	String.Size = Size;
	return String;
}

inline string StringAlloc(char * Text){
	string String;
	String.Size = CStringLenght(Text);
	printf("L: %i", String.Size);
	String.Base = MemAlloc(String.Size + 1);
	MemCopy(String.Base, Text, String.Size);
	return String;
}

inline string StringAllocSubstr(char * Text, s32 Size){
	string String;
	String.Size = Size;
	String.Base = MemAlloc(String.Size + 1);
	MemCopy(String.Base, Text, String.Size);
	return String;
}


inline string StringAllocWithSize(s32 Size){
	string String;
	String.Size = Size;
	String.Base = MemAlloc(String.Size + 1);
	return String;
}

inline void StringFree(string * String){
	MemFree(String->Base);
	String->Size = 0;
}


static b32 StringCompare(string Str1, string Str2){
	if(Str1.Size != Str2.Size){
		return false;	
	}
	for(int i = 0; i < Str1.Size; i++){
		if(Str1.Base[i] != Str2.Base[i]){
			return false;
		}
	}
	return true;
}


static string StringListJoin(string** List, char Token){
	s32 Size = 0;
	for(int i = 0; i < ArraySize(*List); i++){
		string String = (*List)[i];
		Size += String.Size + 1;
	}
	
	Size = (Size) ? Size - 1: Size ;
	string Result = StringAllocWithSize(Size);
	memset(Result.Base, 64, Size);
	s32 Offset = 0;
	for(int i = 0; i < ArraySize(*List); i++){
		if(i && Token){
			Result.Base[Offset] = Token;
			Offset += 1;
		}
		string String = (*List)[i];
		MemCopy(Result.Base + Offset, String.Base, String.Size);
		Offset += String.Size;
		
	}
	return Result;
}


static void StringSplit(string** List, string String, char Token){
	s32 i = 0, Offset = 0;
	while(true){
		if(!String.Base[i]){
			string Entry =  StringCreateWithSize(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			break;
		}
		
		if(String.Base[i] == Token && String.Base[i + 1]){
			string Entry =  StringCreateWithSize(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			Offset = i + 1;
		}
		
		i++;
	}
}

static void StringSemanticSplit(string** List, string String, char Token){
	s32 i = 0, Offset = 0, Quotes = 0;
	
	while(true){
		if(String.Base[i] == '\\' && String.Base[i + 1] == '\"'){
			Quotes = !Quotes;
		}
		
		if(!String.Base[i] && !Quotes){
			string Entry = StringCreateWithSize(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			break;
		}
		
		if(String.Base[i] == Token && String.Base[i + 1]){
			string Entry = StringCreateWithSize(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			Offset = i + 1;
		}
		
		i++;
	}
}

static void StringSemanticSplitAlloc(string** List, string String, char Token){
	s32 i = 0, Offset = 0, Quotes = 0;
	
	while(true){
		if(String.Base[i] == '\\' && String.Base[i + 1] == '\"'){
			Quotes = !Quotes;
		}
		
		if(!String.Base[i] && !Quotes){
			string Entry = StringAllocSubstr(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			break;
		}
		
		if(String.Base[i] == Token && String.Base[i + 1]){
			string Entry = StringAllocSubstr(String.Base + Offset, i - Offset);
			ArrayAppend(List, &Entry);
			Offset = i + 1;
		}
		
		i++;
	}
}

static string StringFormat(string Format, ...){
	va_list arg;
	
	string *List = ArrayAlloc(string);	
	
	s32 Size = Format.Size;
	
#ifndef _MSC_VER
	va_start (arg, Format);
#else
	va_start (arg, &Format);
#endif
	
	for(int i = 0; i < Format.Size; i++){
		if(Format.Base[i] == '%'){
			string Entry = va_arg(arg, string);
			string * String = ArrayAppend(&List, &Entry);
			Size += String->Size - 1;
		}
	}
	va_end (arg);
	
	string Formatted = StringAllocWithSize(Size);
	s32 Offset = 0;
	s32 OffsetDelta = 0;
	u32 ListIndex = 0;
	for(int i = 0; i < Format.Size; i++){
		if(Format.Base[i] == '%'){
			string String = List[ListIndex++];
			MemCopy(Formatted.Base + Offset + OffsetDelta, Format.Base + Offset, i - Offset);
			
			MemCopy(Formatted.Base + Offset + OffsetDelta + i - Offset, String.Base, String.Size);
			
			Offset = i + 1;
			OffsetDelta += String.Size - 1;
		}
	}
	
	MemCopy(Formatted.Base + Offset + OffsetDelta, Format.Base + Offset, Format.Size - Offset);
	
	ArrayFree(List);
	return Formatted;
}
