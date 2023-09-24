#include "lexer.h"
#include <fstream>
#include <iostream>

//TODO: this must be changeable at run-time
#define TAB_SIZE 4

Lexer::Lexer(const std::string& filename) : filename_(filename) {
    tokens = TokenVector();
}

Lexer::~Lexer() {
#if DEBUG
    printf("LEXER DESTROYED");
#endif
}

TokenVector& Lexer::tokenize() {
    // Open the .gd file
    std::ifstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename_ << std::endl;
        return tokens;
    }

    std::string source_code;
    // Loop through each character in the file
    char ch;
    line = 1;
    column = 0;
    ident_level = 0;
    source_code.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    for (int i = 0; i <= source_code.length(); i++)
    {
        ch = source_code[i];
        column++;
        Token token;

        switch(ch) {
            case '\n':
                line++;
                column = 0;
                ident_level = 0;
            continue;

            case '\t':
                ident_level++;
				column += TAB_SIZE - 1;
            continue;

            case ' ':
            
            continue;

            case '-':
                if (source_code[i+1] == '>') {
                    i++;
                    make_token(TokenType::FORWARD_ARROW, "->");
                    column++;
                }
                else {
                    make_token(TokenType::MINUS, "-");
                }
            continue;

            case '>':
                make_token(TokenType::GREATER_THAN, ">");
            continue;

            case '(':
                make_token(TokenType::PARENTHESIS_OPEN, "(");
            continue;

            case ')':
                make_token(TokenType::PARENTHESIS_CLOSE, ")");

            continue;

            case ':':
                make_token(TokenType::COLON, ":");

            continue;

            case ';':

            continue;

            case '\0':
                make_token(TokenType::TK_EOF, "EOF");
            #if DEBUG
                printf("END OF FILE REACHED\n");
            #endif
            continue;


        }


        if (ch == '\"') {
            char c = ch;
            std::string word = "";
            while(true) {
                word += c;
                i++;
                c = source_code[i];
                if (c == '\"') {
                    word += '\"';
                    make_token(TokenType::STRING, word);
                    break;
                }
            }
            std::cout << "found string: " + word + "\n";
            continue;
        } else
        if (check_isdigit(&ch)) {
            printf("digit\n");
            
            continue;
        } else if (check_isalnum(&ch)) {
            std::string word = "";
            char c = ch;
            
            while(check_isalnum(&c)) {
                word += c;
                i++;
                c = source_code[i];
            }

            auto contains = keyword_map.find(word);

            if (contains != keyword_map.end()) {
                make_token(contains->second, word);
            } else {
                make_token(TokenType::IDENTIFIER, word);
            }


            column += word.length()-1;
            i--;

           

            // std::cout << "found identifier: " + word + "\n";
            
            continue;
        }
    #if DEBUG
        std::cout << "missed token: " << ch << "\n";
    #endif

    }



    // Close the file
    file.close();

    return tokens;
}

bool Lexer::check_isalnum(const char *c) {
    return (*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9') || (*c == '_');
}

bool Lexer::check_isdigit(const char *c) {
    return (*c >= '0' && *c <= '9');
}

bool Lexer::match_next(char ch) {
    return false;
}

void Lexer::make_token(TokenType type, std::string lexeme) {
    Token token;
    token.column = column;
    token.ident_level = ident_level;
    token.lexeme = lexeme;
    token.line = line;
    token.scope = UNDEFINED;
    token.type = type;
#if DEBUG
    print_token(token);
#endif
    tokens.push_back(token);
}

#if DEBUG
void Lexer::print_token(const Token& token) {
    std::cout << "=== TOKEN ===\n";
    std::cout << "Type: " << STR_TOKEN_TYPE[token.type] << "\n";
    std::cout << "Lexeme: " << token.lexeme << "\n";
    std::cout << "line: " << token.line << "\n";
    std::cout << "column: " << token.column << "\n";
    std::cout << "identation: " << token.ident_level << "\n";
    std::cout << "Scope: " << "UNDEFINED\n";
    std::cout << "=== END TOKEN ===\n\n";
}

#endif