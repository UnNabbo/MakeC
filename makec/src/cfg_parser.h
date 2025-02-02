
typedef enum{
    TOKEN_TYPE_INVALID,

    TOKEN_TYPE_COLON,
	
    TOKEN_TYPE_EQUAL,
    TOKEN_TYPE_SEMI_COLON,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_OPEN_BRACE,
    TOKEN_TYPE_CLOSE_BRACE,
    TOKEN_TYPE_OPEN_SQUARE_BRACE,
    TOKEN_TYPE_CLOSE_SQUARE_BRACE,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_IDENTIFIER,
	TOKEN_TYPE_STRING_LITERAL,
	TOKEN_TYPE_INLINE_COMMENT,
    TOKEN_TYPE_,
	
    TOKEN_TYPE_COUNT,
} token_type;

typedef struct{
    char * Literal;
    u32 Enum;
} keyword;

typedef struct{
	u32 Line;
	char * File;
} debug_info;

typedef struct {
    u32 Enum;
    
    union{
		string Literal;
        keyword Keyword;
    };
	debug_info Info;
} token;

typedef enum{
    KEYWORD_TYPE_INVALID,

	KEYWORD_TYPE_PROJECT, 
	KEYWORD_TYPE_DEPENDENCY, 
    KEYWORD_TYPE_COMPILER, 
    KEYWORD_TYPE_PATH, 
    KEYWORD_TYPE_VERSION, 
    KEYWORD_TYPE_OUT, 
    KEYWORD_TYPE_KIND, 
    KEYWORD_TYPE_FILES, 
    KEYWORD_TYPE_LIBS, 
    KEYWORD_TYPE_LIB_DIRECTORIES, 
    KEYWORD_TYPE_INCLUDE, 
    KEYWORD_TYPE_FLAGS, 
    KEYWORD_TYPE_LINK, 
    KEYWORD_TYPE_EXPORT, 
    KEYWORD_TYPE_DEFINE,
	
    KEYWORD_TYPE_MSVC,
	
    KEYWORD_TYPE_EXE, 
    KEYWORD_TYPE_LIB, 
    KEYWORD_TYPE_DLL, 
	
    KEYWORD_TYPE_TYPE_COUNT,
} keyword_type;


typedef struct{
    char * Literal;
    u32 Enum;
} symbol;

#define CaseEnumToString(Enum) case Enum: return #Enum

char * TokenTypeToString(u32 Enum);
void TokenPrint(token Token);
token ParserExpectNext(token **Tokens, s32 * Offset, u32 Enum, b32 * Result);
token ParserFetchNext(token **Tokens, s32 * Offset);
project * TokenParseIntoProject(token** Tokens);
token TokenizerExtractSymbolToken(char ** Data);
token TokenizerExtractToken(char * Literal, s32 Lenght);
void TokenizerAdvanceTillValidChar(char ** Data);
token * Tokenize(char * Path);
