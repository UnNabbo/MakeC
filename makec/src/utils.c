
static void Printsl(string Text){
	for(int i = 0; i < Text.Size; i++){
		putchar(Text.Base[i]);
	}
}

static void Print(string Text){
	for(int i = 0; i < Text.Size; i++){
		putchar(Text.Base[i]);
	}
	putchar('\n');
}

static void Printf(string Format, ...){
	va_list arg;
	
	string * List = ArrayAlloc(string);	
	
	s32 Size = Format.Size;
	
#ifndef _MSC_VER
	va_start (arg, Format);
#else
	va_start (arg, &Format);
#endif
	
	for(int i = 0; i < Format.Size; i++){
		if(Format.Base[i] == '%'){
			string Entry = va_arg(arg, string);
			ArrayAppend(&List, &Entry);
			Size += Entry.Size - 1;
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
	
	Printsl(Formatted);
	
	StringFree(&Formatted);
}
