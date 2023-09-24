#include <iostream>


#include "lexer.h"
#include "parser.h"

int main()
{
    
    Lexer lexer("debug.gd");
    TokenVector& tokens = lexer.tokenize();

    Parser parser;
    parser.parse(tokens);

    

    return 0;
}