
keyword Keywords[] = {
	{"@project", KEYWORD_TYPE_PROJECT}, 
	//{"@dependency", KEYWORD_TYPE_DEPENDENCY}, 
    {"@compiler", KEYWORD_TYPE_COMPILER }, 
    {"@path", KEYWORD_TYPE_PATH  },
    //{"@version", KEYWORD_TYPE_VERSION  },
    {"@out", KEYWORD_TYPE_OUT  },
    {"@kind", KEYWORD_TYPE_KIND  },
    {"@files", KEYWORD_TYPE_FILES  },
    {"@libs", KEYWORD_TYPE_LIBS  },
    {"@lib_directories", KEYWORD_TYPE_LIB_DIRECTORIES }, 
    {"@include", KEYWORD_TYPE_INCLUDE  },
    {"@flags", KEYWORD_TYPE_FLAGS  },
    {"@link", KEYWORD_TYPE_LINK  },
    {"@export", KEYWORD_TYPE_EXPORT  },
    {"@define", KEYWORD_TYPE_DEFINE 	 },
};

symbol Symbols[] = {
    {"=", TOKEN_TYPE_EQUAL},
    {";", TOKEN_TYPE_SEMI_COLON},
    {",", TOKEN_TYPE_COMMA},
    {"{", TOKEN_TYPE_OPEN_BRACE},
    {"}", TOKEN_TYPE_CLOSE_BRACE},
    {"[", TOKEN_TYPE_OPEN_SQUARE_BRACE},
    {"]", TOKEN_TYPE_CLOSE_SQUARE_BRACE},
    {"\"", TOKEN_TYPE_STRING_LITERAL},
    {"//", TOKEN_TYPE_INLINE_COMMENT},
};

const s32 SymbolCount = sizeof(Symbols) / sizeof(Symbols[0]);
const s32 KeyWordCount = sizeof(Keywords) / sizeof(Keywords[0]);

static b32 CharacterIsValid(char Char){
	return ( Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'Z') || (Char >= '0' && Char <= '9') || Char == '@';
}

char * TokenTypeToString(u32 Enum){
    switch(Enum){
        CaseEnumToString(TOKEN_TYPE_INVALID);
        
        CaseEnumToString(TOKEN_TYPE_COLON);
        CaseEnumToString(TOKEN_TYPE_KEYWORD);
        CaseEnumToString(TOKEN_TYPE_IDENTIFIER);
        CaseEnumToString(TOKEN_TYPE_INLINE_COMMENT);
        CaseEnumToString(TOKEN_TYPE_STRING_LITERAL);
        CaseEnumToString(TOKEN_TYPE_CLOSE_SQUARE_BRACE);
        CaseEnumToString(TOKEN_TYPE_OPEN_SQUARE_BRACE);
        CaseEnumToString(TOKEN_TYPE_CLOSE_BRACE);
        CaseEnumToString(TOKEN_TYPE_OPEN_BRACE);
        CaseEnumToString(TOKEN_TYPE_COMMA);
        CaseEnumToString(TOKEN_TYPE_SEMI_COLON);
        CaseEnumToString(TOKEN_TYPE_EQUAL);
        //CaseEnumToString(TOKEN_TYPE_);
        default: return 0;
    }
}

static void TokenPrint(token Token){
    printf("%s", TokenTypeToString(Token.Enum));
    switch(Token.Enum){
        case TOKEN_TYPE_KEYWORD:{
            printf(": %s", Token.Keyword.Literal);
        }break;
		case TOKEN_TYPE_STRING_LITERAL:{
            Printf(STR(": %"), Token.Literal);
        }break;
        case TOKEN_TYPE_IDENTIFIER:{
            printf(": %s", Token.Literal.Base);
        }break;
    }
	printf("\n");
}

token ParserExpectNext(token **Tokens, s32 * Offset, u32 Enum, b32 * Result){
	token Token;
	ZeroMemory(Token);
	if(*Offset + 1 <= ArraySize(*Tokens)){
		Token = (*Tokens)[(*Offset)++];
	}

	if(Result && Token.Enum != Enum){
		
		*Result = false;
		printf("[%s]: Was expexting token of type %s but got %s\n", Token.Info.File, TokenTypeToString(Enum), TokenTypeToString(Token.Enum));
	}

	return Token;
}


token ParserExpectNextKeyword(token **Tokens, s32 * Offset, u32 Enum, b32 * Result){
	token Token = ParserExpectNext(Tokens, Offset, TOKEN_TYPE_KEYWORD, Result);
	if(Result){
		if(*Result  && Token.Keyword.Enum != Enum){
			printf("[%s][<WIP>]: Was expexting token keyword of type <WIP> but got <WIP>\n", Token.Info.File);
			*Result = false;
		}
	}
	return Token;
}

token ParserFetchNext(token **Tokens, s32 * Offset){
	token Token;
	ZeroMemory(Token);
	if(*Offset + 1 <= ArraySize(*Tokens)){
		Token = (*Tokens)[(*Offset)++];
	}

	return Token;
}


token ParserPeekNext(token **Tokens, s32 * Offset){
	token Token;
	ZeroMemory(Token);
	if(*Offset + 1 <= ArraySize(*Tokens)){
		Token = (*Tokens)[(*Offset) + 1];
	}

	return Token;
}

#define ParserFormatProjectFields_PANIC(value) if(value) {printf("[%s]: Field %s was already defined\n", Field.Info.File, Field.Keyword.Literal); return; }

void ParserFormatProjectFields(project * Project, token Field, token Value){
	if(Field.Keyword.Enum == KEYWORD_TYPE_COMPILER){
		ParserFormatProjectFields_PANIC(Project->Compiler);
		if(StringCompare(Value.Literal, STR("msvc"))){
			Project->Compiler = COMPILER_MSVC;
		}
	}else if(Field.Keyword.Enum == KEYWORD_TYPE_KIND){
		ParserFormatProjectFields_PANIC(Project->Output);
		if(StringCompare(Value.Literal, STR("exe"))){
			Project->Output = OUTPUT_EXECUTABLE;
		}else if(StringCompare(Value.Literal, STR("lib"))){
			Project->Output = OUTPUT_LIBRARY;
		}else if(StringCompare(Value.Literal, STR("dll"))){
			Project->Output = OUTPUT_DYNAMIC_LIBRARY;
		}
	}else{
		if(Value.Enum != TOKEN_TYPE_IDENTIFIER || Value.Enum != TOKEN_TYPE_STRING_LITERAL){
			switch(Field.Keyword.Enum){
				case KEYWORD_TYPE_PATH:{
					ParserFormatProjectFields_PANIC(Project->FilePath);
					Project->FilePath = Value.Literal.Base;
				}break;
				case KEYWORD_TYPE_VERSION:{
					//WIP
				}break;
				case KEYWORD_TYPE_OUT:{
					ParserFormatProjectFields_PANIC(Project->OutputPath);
					Project->OutputPath = CStringAlloc(Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_DEPENDENCY:{
					ProjectAddDependency(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_FILES:{
					ProjectAddFiles(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_LIBS:{
					ProjectLinkLibs(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_LIB_DIRECTORIES:{
					ProjectAddLibsDirs(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_INCLUDE:{
					ProjectAddIncludeDirs(Project, Value.Literal.Base);
				}break;				
				case KEYWORD_TYPE_FLAGS:{
					ProjectAddCompilerFlags(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_LINK:{
					ProjectAddLinkerFlags(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_EXPORT:{
					ProjectExportSymbols(Project, Value.Literal.Base);
				}break;
				case KEYWORD_TYPE_DEFINE:{
					ProjectAddDefines(Project, Value.Literal.Base);
				}break;
			}
 		}
	}
}
b32 ParserParseProject(project * Project, token** Tokens, s32* Offset){
	b32 Result = true;
	token ProjectKeyword = ParserExpectNextKeyword(Tokens, Offset, KEYWORD_TYPE_PROJECT, &Result);
	token Name = ParserExpectNext(Tokens, Offset, TOKEN_TYPE_STRING_LITERAL, &Result);
	Project->Name = Name.Literal.Base;
	ParserExpectNext(Tokens, Offset, TOKEN_TYPE_EQUAL, &Result);
	ParserExpectNext(Tokens, Offset, TOKEN_TYPE_OPEN_BRACE, &Result);
	while(Result && *Offset < ArraySize(*Tokens)){
		token Field = ParserFetchNext(Tokens, Offset);
		if(Field.Enum == TOKEN_TYPE_CLOSE_BRACE) break;
		if(Field.Enum != TOKEN_TYPE_KEYWORD){
			printf("[%s][<WIP>]: Was expecting a field but got a %\n", Field.Info.File, TokenTypeToString(Field.Enum));
			return false;
		};
		ParserExpectNext(Tokens, Offset, TOKEN_TYPE_EQUAL, &Result);
		token Assignment = ParserFetchNext(Tokens, Offset);
		switch(Assignment.Enum){
			case TOKEN_TYPE_STRING_LITERAL:{
				ParserFormatProjectFields(Project, Field, Assignment);
			}break;
			case TOKEN_TYPE_OPEN_SQUARE_BRACE:{
				if(Field.Keyword.Enum == KEYWORD_TYPE_OUT || Field.Keyword.Enum == KEYWORD_TYPE_PATH || Field.Keyword.Enum == KEYWORD_TYPE_KIND || Field.Keyword.Enum == KEYWORD_TYPE_COMPILER){
					printf("[%s][<WIP>]: Field of type %s does not support array assignment\n", Field.Info.File, Field.Keyword.Literal);
					return false;
				}
				while(Result){
					token Literal = ParserExpectNext(Tokens, Offset, TOKEN_TYPE_STRING_LITERAL, &Result);
					ParserFormatProjectFields(Project, Field, Literal);
					token Symbol = ParserFetchNext(Tokens, Offset);
					if(Symbol.Enum == TOKEN_TYPE_CLOSE_SQUARE_BRACE) break;
					if(Symbol.Enum != TOKEN_TYPE_COMMA){
						printf("[%s][<WIP>]: Was expecting a comma but got a %\n", Symbol.Info.File, TokenTypeToString(Symbol.Enum));
						return false;
					}
				}
			}break;
		}
		
		ParserExpectNext(Tokens, Offset, TOKEN_TYPE_SEMI_COLON, &Result);
	}
	
	return Result;
}

project * TokenParseIntoProject(token** Tokens){
	project * Projects = ArrayAlloc(project);

	for(int Offset = 0; Offset < ArraySize(*Tokens);){
		project Project;
		ZeroMemory(Project);
		
		Project.SourceFiles = ArrayAlloc(string);
		Project.Libs = ArrayAlloc(string);
		Project.LibsDirs = ArrayAlloc(string);
		Project.IncludeDirs = ArrayAlloc(string);
		Project.CompilerFlags = ArrayAlloc(string);
		Project.LinkerFlags = ArrayAlloc(string);
		Project.Symbols = ArrayAlloc(string); 
		Project.Defines = ArrayAlloc(string);
		Project.Dependency = ArrayAlloc(string);
		
		if(!ParserParseProject(&Project, Tokens, &Offset)){
			ArrayFree(Projects);
			return 0;
		}
		ArrayAppend(&Projects, &Project);
	}
	return Projects;
}

token TokenizerExtractSymbolToken(char ** Data){
    token Token;
    ZeroMemory(Token);
    s32 CandidateSize = 0;
    for(int i = 0; i < SymbolCount; i++){
        s32 Len = strlen(Symbols[i].Literal);
        if(CStringCompareN(*Data, Symbols[i].Literal, Len) && Len > CandidateSize){
            Token.Enum = Symbols[i].Enum;
            CandidateSize = Len;
        }
    }
    
    (*Data) += CandidateSize;
    return Token;
}

token TokenizerExtractToken(char * Literal, s32 Lenght){
    token Token;
    ZeroMemory(Token);

    for(int i = 0; i < KeyWordCount; i++){
        if(CStringCompare(Literal, Keywords[i].Literal)){
            Token.Enum = TOKEN_TYPE_KEYWORD;
            Token.Keyword = Keywords[i];
            return Token;
        }
    }
        
    Token.Enum = TOKEN_TYPE_IDENTIFIER;
    Token.Literal = STR(Literal);
    return Token;
}

void TokenizerAdvanceTillValidChar(char ** Data){
    while(true){
        if((*Data)[0] == ' ' ||
           (*Data)[0] == '\n' ||
           (*Data)[0] == '\r' ||
           (*Data)[0] == 9)
        {
            (*Data)++;

            continue;
        }
        break;
    }
}

static token * Tokenize(char * Path){
	FILE * FileHandle = fopen(Path, "rb");
	fseek(FileHandle, 0, SEEK_END);
    u64 FileSize = ftell(FileHandle);
    fseek(FileHandle, 0, SEEK_SET);
	char *Text = MemAlloc(FileSize + 1);
    fread(Text, FileSize, 1, FileHandle);
    Text[FileSize] = 0;
    fclose(FileHandle);
	char * Data = Text;
	token* Tokens = ArrayAlloc(token);
       
    token Token;
	debug_info Info;
	Info.File = CStringAlloc(Path);
	Info.Line = 0;
    while(Data[0]){
        ZeroMemory(Token);
		TokenizerAdvanceTillValidChar(&Data);
        Token.Info = Info;
		s32 Size = 0;
        Token = TokenizerExtractSymbolToken(&Data);
        if(Token.Enum != TOKEN_TYPE_INVALID){
			switch(Token.Enum){
				case TOKEN_TYPE_STRING_LITERAL:{
					while(Data[Size] != '"' && Data[Size++] != 0){}
					char * Literal = MemAlloc(Size + 1);
					MemCopy(Literal, Data, Size);
					Data += Size + 1;
					Token.Literal = STR(Literal);
					ArrayAppend(&Tokens, &Token);
					continue;
				}break;
				case TOKEN_TYPE_INLINE_COMMENT:{
					while(Data[Size] != '\n' && Data[Size++] != 0){}
					Data += Size;
					continue;
				}break;
				default:{
					ArrayAppend(&Tokens, &Token);
					continue;
				}break; 
			}
		}
        
		while(true){
			if(Data[Size] != ' ' &&
			   Data[Size] != '\n' &&
			   Data[Size] != '\r' &&
			   Data[Size] != 9 &&
			   (CharacterIsValid(Data[Size])))
			{
				Size++;
				continue;
			}
			break;
		}
        
		if(Size){
			char * Literal = MemAlloc(Size + 1);
			MemCopy(Literal, Data, Size);
			Data += Size;
			Token = TokenizerExtractToken(Literal, Size);
		}
		if(Token.Enum != TOKEN_TYPE_INVALID){
			ArrayAppend(&Tokens, &Token);
		}else{
			printf("Unexpected token %c encoutured while lexing file: %s\n", Data[0], Path);
			Data++;
			continue;
		}
	}

	MemFree(Text);
	return Tokens;
}
