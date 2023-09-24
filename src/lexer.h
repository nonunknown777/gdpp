#ifndef GD_LEXER
#define GD_LEXER

#include <string>
#include <map>
#include <vector>

enum TokenType {
    IDENTIFIER,
    FUNC,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    COLON,
    VOID,
    MINUS,
    GREATER_THAN,
    FORWARD_ARROW,
    EXTENDS,
    QUOTE,
    STRING,
    CLASS_NAME,
    KEYWORD,
    PASS,

    TK_EOF,
    TK_MAX


};

const char* const STR_TOKEN_TYPE[17] = {
    "IDENTIFIER",
    "FUNC",
    "PARENTHESIS_OPEN",
    "PARENTHESIS_CLOSE",
    "COLON",
    "VOID",
    "MINUS",
    "GREATER_THAN",
    "FORWARD_ARROW",
    "EXTENDS",
    "QUOTE",
    "STRING",
    "CLASS_NAME",
    "KEYWORD",
    "PASS",

    "TK_EOF",
    "TK_MAX"
};

inline std::string print_token_type(TokenType t) { return std::string(STR_TOKEN_TYPE[t]);}

enum Scope {
    UNDEFINED,
    GLOBAL,
    LOCAL,
    FUNCTION
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    int ident_level;
    Scope scope; //TODO: its really necessary? maybe yes, but should be in another place??????
};

const std::map<std::string, TokenType> keyword_map = {
    {   "func",         TokenType::FUNC                     },
    {   "(",            TokenType::PARENTHESIS_OPEN         },
    {   ")",            TokenType::PARENTHESIS_CLOSE        },
    {   ":",            TokenType::COLON                    },
    {   "void",         TokenType::VOID                     },
    {   "-",            TokenType::MINUS                    },
    {   ">",            TokenType::GREATER_THAN             },
    {   "->",           TokenType::FORWARD_ARROW            },
    {   "extends",      TokenType::EXTENDS                  },
    {   "\"",           TokenType::QUOTE                    },
    {   "class_name",   TokenType::CLASS_NAME               },
    {   "pass",         TokenType::PASS                     }
};

enum DataType {
    VOID_T,
    STRING_T
};

const std::map<std::string, DataType> data_type_map = {
    {   "void",         DataType::VOID_T                    },
    {   "string",       DataType::STRING_T                  }
};

typedef std::vector<Token> TokenVector;

class Lexer {
public:
    Lexer(const std::string& filename);
    ~Lexer();

    TokenVector& tokenize();
    TokenVector tokens;


private:
    std::string filename_;
    int line;
    int column;
    int ident_level;
    bool check_isalnum(const char *ch);
    bool check_isdigit(const char *ch);
    bool match_next(char ch);
    void make_token(TokenType type, std::string lexeme);

#if DEBUG
    void print_token(const Token& token);
#endif

};

#endif // GD_LEXER
