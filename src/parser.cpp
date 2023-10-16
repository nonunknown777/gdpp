
#include <iostream>

#include "parser.h"

#define ERR_MSG(token, msg) \
        error = new Error(token, msg); \
        push_error();

#define CHECK_IDENTIFIER(token) \
        if (current_token->type != TokenType::IDENTIFIER) { \
        std::string msg = "Expected " + print_token_type(TokenType::IDENTIFIER) + " but found: " + print_token_type(current_token->type) + " \""+current_token->lexeme+"\""; \
        error = new Error(token, msg); \
        push_error(); \
        return nullptr; \
        }

void Parser::parse(TokenVector& tokens) {
    this->tokens = &tokens;
    index = 0;
    Script script = Script();
    Block body = Block();
    script.body = &body;
    NodePTR script_ptr = NodePTR(NodeType::SCRIPT, &script);
    
    create_script(script);

    create_nodes(script_ptr, body);
}

void Parser::create_script(Script& scr) {

    for (size_t i = 0; i < tokens->size(); i++)
    {
        current_token = &tokens->at(index);
        switch(current_token->type) {

            case TokenType::CLASS_NAME:
              scr.class_declaration = create_node_class_declaration();
            break;

            case TokenType::EXTENDS:
             scr.parent = create_inheritance();
            break;

            default:
                ERR_MSG(*current_token, "Script must have class_name and extends before body");
            break;
        }

        consume_next();
        i = index;
    }
}

void Parser::create_nodes(const NodePTR& p_owner, Block& p_block) {

    for (size_t i = 0; i < tokens->size(); i++)
    {
        current_token = &tokens->at(index);
        switch(current_token->type) {

            case TokenType::FUNC:
                if (p_owner.node_type != NodeType::SCRIPT) {
                    ERR_MSG(*current_token, "is not valid at this context");
                    break; //TODO: maybe should be return? but continuing is better for gathering more errors
                }
                NodePTR function = NodePTR(NodeType::FUNCTION_DECLARATION, create_function());
                p_block.nodes.push_back(function);
            break;

            

            default:
                ERR_MSG(*current_token, "invalid/unimplemented token");
            break;
        }

        consume_next();
        i = index;
    }
}

// Go to next token in the vector
void Parser::consume_next() {
    //TODO: check for end token
    // if (index+1 <= tokens->size())
    index++;
    current_token = &tokens->at(index);
}


ClassDeclaration* Parser::create_node_class_declaration() {

    //Rule: ClassDefinition ::= "class_name" Identifier

    ClassDeclaration* cd = new ClassDeclaration("");

    consume_next();

    CHECK_IDENTIFIER(*current_token);


    cd->class_name = current_token->lexeme;
    return cd;
}

Inheritance* Parser::create_inheritance() {

    //Rule: Inheritance ::= "extends" Identifier

    Inheritance* in = new Inheritance("");
    consume_next();
    
    CHECK_IDENTIFIER(*current_token);

    in->parent_name = current_token->lexeme;

    return in;
}

FunctionDeclaration* Parser::create_function() {

    //Rule: FunctionDeclaration ::= "func" Identifier + "(" Parameter[] ")" "->" ReturnType ":" Block "pass"

    FunctionDeclaration* func = new FunctionDeclaration("");
    check_next(TokenType::IDENTIFIER);

    func->function_name = current_token->lexeme;

    check_next(TokenType::PARENTHESIS_OPEN);

    func->args = std::vector<Parameter*>(); //TODO: must not be a pointer?

    consume_next();
    while(current_token->type != PARENTHESIS_CLOSE) {
        //TODO: Implement function arguments
        consume_next();
    }

    if (func->args.size() == 0) {
        printf("empty function declaration");
    } else {
        printf("TODO: what has to be done with functions arguments?");
    }

    check_next(TokenType::FORWARD_ARROW);

    check_valid_return_type();

    DataType dt = data_type_map.at(current_token->lexeme);

    ReturnType* rt = new ReturnType(dt);
    func->return_type = rt;

    check_next(TokenType::COLON);

    Block function_body = Block();

    NodePTR ptr = NodePTR(NodeType::FUNCTION_DECLARATION, func);

    create_nodes(ptr, function_body);

    check_next(TokenType::PASS);

    return func;

}



void Parser::check_valid_return_type() {
    consume_next();

    auto it = data_type_map.find(current_token->lexeme);

    if (it == data_type_map.end()) {
        std::string msg = "Invalid return type: " + print_token_type(current_token->type);
        error = new Error(*current_token, msg);
        push_error();
    }
}

void Parser::check_next(TokenType token_type) {
    consume_next();
    if (current_token->type != token_type) {
        std::string msg = "Expected " + print_token_type(token_type) + " but found: " + print_token_type(current_token->type) + " \""+current_token->lexeme+"\"";
        error = new Error(*current_token, msg);
        push_error();
    }
}

Parser::Parser() {
    tokens = nullptr;
    current_token = nullptr;
    error = nullptr;
    // errors = std::vector<Error>();
}

inline void Parser::push_error() {
    if (error != nullptr) {
        std::cerr << error->msg;
        throw std::exception();
    }
}