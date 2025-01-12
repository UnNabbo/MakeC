
keyword Keywords[] = {
	{"project", KEYWORD_TYPE_PROJECT }, 
    {"compiler", KEYWORD_TYPE_COMPILER }, 
    {"path", KEYWORD_TYPE_PATH  },
    {"version", KEYWORD_TYPE_VERSION  },
    {"out", KEYWORD_TYPE_OUT  },
    {"kind", KEYWORD_TYPE_KIND  },
    {"files", KEYWORD_TYPE_FILES  },
    {"libs", KEYWORD_TYPE_LIBS  },
    {"lib_directories", KEYWORD_TYPE_LIB_DIRECTORIES }, 
    {"include", KEYWORD_TYPE_INCLUDE  },
    {"flags", KEYWORD_TYPE_FLAGS  },
    {"link", KEYWORD_TYPE_LINK  },
    {"export", KEYWORD_TYPE_EXPORT  },
    {"define", KEYWORD_TYPE_DEFINE 	 },
	
    {"exe", KEYWORD_TYPE_EXE 	 },
    {"lib", KEYWORD_TYPE_LIB 	 },
    {"dll", KEYWORD_TYPE_DLL 	 },
	
    {"msvc", KEYWORD_TYPE_MSVC},
};

symbol Symbols[] = {
    {":", TOKEN_TYPE_COLON},
    {"\"", TOKEN_TYPE_STRING_LITERAL},
    {"//", TOKEN_TYPE_INLINE_COMMENT},
};

const s32 SymbolCount = sizeof(Symbols) / sizeof(Symbols[0]);
const s32 KeyWordCount = sizeof(Keywords) / sizeof(Keywords[0]);


char * TokenTypeToString(u32 Enum){
    switch(Enum){
        CaseEnumToString(TOKEN_TYPE_INVALID);
        
        CaseEnumToString(TOKEN_TYPE_COLON);
        CaseEnumToString(TOKEN_TYPE_KEYWORD);
        CaseEnumToString(TOKEN_TYPE_IDENTIFIER);
        CaseEnumToString(TOKEN_TYPE_INLINE_COMMENT);
        CaseEnumToString(TOKEN_TYPE_STRING_LITERAL);
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

project TokenParseIntoProject(token** Tokens, char * FilePath){
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

	for(int Offset = 0; Offset < ArraySize(*Tokens);){
		b32 Result = true;
		token KeywordToken = ParserExpectNext(Tokens, &Offset, TOKEN_TYPE_KEYWORD, &Result);
		ParserExpectNext(Tokens, &Offset, TOKEN_TYPE_COLON, &Result);
		if(KeywordToken.Keyword.Enum == KEYWORD_TYPE_COMPILER){
			token Value = ParserExpectNext(Tokens, &Offset, TOKEN_TYPE_KEYWORD, &Result);
			switch(Value.Keyword.Enum){
				case KEYWORD_TYPE_MSVC:{
					Project.Compiler = COMPILER_MSVC;
				}break;
				default:{
					Result = false;
				}break;
			}
		}else if(KeywordToken.Keyword.Enum == KEYWORD_TYPE_KIND){
			token Value = ParserExpectNext(Tokens, &Offset, TOKEN_TYPE_KEYWORD, &Result);
			switch(Value.Keyword.Enum){
				case KEYWORD_TYPE_EXE:{
					Project.Output = OUTPUT_EXECUTABLE;
				}break;
				case KEYWORD_TYPE_LIB:{
					Project.Output = OUTPUT_LIBRARY;
				}break;
				case KEYWORD_TYPE_DLL:{
					Project.Output = OUTPUT_DYNAMIC_LIBRARY;
				}break;
				default:{
					Result = false;
				}break;
			}
		}else{
			token Value = ParserFetchNext(Tokens, &Offset);
			if(Value.Enum != TOKEN_TYPE_IDENTIFIER || Value.Enum != TOKEN_TYPE_STRING_LITERAL && Result){
				switch(KeywordToken.Keyword.Enum){
					case KEYWORD_TYPE_PROJECT:{
						Project.Name = CStringAlloc(Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_PATH:{
						Project.FilePath = CStringAlloc(Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_VERSION:{
						//WIP
					}break;
					case KEYWORD_TYPE_OUT:{
						Project.OutputPath = CStringAlloc(Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_FILES:{
						ProjectAddFiles(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_LIBS:{
						ProjectLinkLibs(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_LIB_DIRECTORIES:{
						ProjectAddLibsDirs(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_INCLUDE:{
						ProjectAddIncludeDirs(&Project, Value.Literal.Base);
					}break;				
					case KEYWORD_TYPE_FLAGS:{
						ProjectAddCompilerFlags(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_LINK:{
						ProjectAddLinkerFlags(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_EXPORT:{
						ProjectExportSymbols(&Project, Value.Literal.Base);
					}break;
					case KEYWORD_TYPE_DEFINE:{
						ProjectAddDefines(&Project, Value.Literal.Base);
					}break;
				}
 			}else{
				Result = false;
			} 
		}
		
		if(!Result){
			printf("Unexpected symbol while parsing %s\n", FilePath);
			break;
		}
	}
	return Project;
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

static token * Tokenize(char * Path, char * Data){
	token* Tokens = ArrayAlloc(token);
       
    token Token;
    while(Data[0]){
        TokenizerAdvanceTillValidChar(&Data);
        ZeroMemory(Token);
        
		s32 Size = 0;
        Token = TokenizerExtractSymbolToken(&Data);
        if(Token.Enum != TOKEN_TYPE_INVALID){
			switch(Token.Enum){
				case TOKEN_TYPE_STRING_LITERAL:{
					while(Data[++Size] != '"' && Data[Size] != 0){}
					char * Literal = MemAlloc(Size + 1);
					MemCopy(Literal, Data, Size);
					Data += Size + 2;
					Token.Literal = STR(Literal);
					ArrayAppend(&Tokens, &Token);
					continue;
				}break;
				case TOKEN_TYPE_INLINE_COMMENT:{
					while(Data[++Size] != '\n' && Data[Size] != 0){}
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
	return Tokens;
}
