
#include <iostream>

#include "parser.h"

void Parser::parse(TokenVector& tokens) {
    this->tokens = &tokens;
    index = 0;
    Script script = Script();
    NodePTR script_ptr = NodePTR(NodeType::SCRIPT, &script);
    
    create_nodes(script_ptr);
}

void Parser::create_nodes(NodePTR& ptr) {

    enum Context {
        SCRIPT_BODY,
        FUNCTION_BODY
    };

    Context context;

    if (ptr.node_type == NodeType::SCRIPT) context = SCRIPT_BODY;
    if (ptr.node_type == NodeType::FUNCTION_DECLARATION) context = FUNCTION_BODY; 

    for (size_t i = 0; i < tokens->size(); i++)
    {
        current_token = &tokens->at(index);
        switch(current_token->type) {

            case TokenType::CLASS_NAME:
                if (context == SCRIPT_BODY)
                //TODO: free cd
                cd = create_node_class_declaration();
                push_error();
                script.class_declaration = cd;
            break;

            case TokenType::EXTENDS:
                //TODO: free in
                in = create_inheritance();
                push_error();
                script.parent = in;
            break;

            case TokenType::FUNC:
                //TODO: free func
                func = create_function();
                push_error();
                
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
    if (current_token->type != TokenType::IDENTIFIER) {
        std::string msg = "Expected " + print_token_type(TokenType::IDENTIFIER) + " but found: " + print_token_type(current_token->type) + " \""+current_token->lexeme+"\"";
        error = new Error(*current_token, msg);

        return nullptr;
        
    }


    cd->class_name = current_token->lexeme;
    return cd;
}

Inheritance* Parser::create_inheritance() {

    //Rule: Inheritance ::= "extends" Identifier

    Inheritance* in = new Inheritance("");
    consume_next();
    if (current_token->type != TokenType::IDENTIFIER) {
        std::string msg = "Expected " + print_token_type(TokenType::IDENTIFIER) + " but found: " + print_token_type(current_token->type) + " \""+current_token->lexeme+"\"";
        error = new Error(*current_token, msg);

        return nullptr;
    }

    in->parent_name = current_token->lexeme;

    return in;
}

FunctionDeclaration* Parser::create_function() {

    //Rule: FunctionDeclaration ::= "func" Identifier + "(" Parameter[] ")" "->" Return ":" Block "pass"

    FunctionDeclaration* func = new FunctionDeclaration("");
    check_next(TokenType::IDENTIFIER);

    func->function_name = current_token->lexeme;

    check_next(TokenType::PARENTHESIS_OPEN);

    func->args = std::vector<Parameter*>();

    consume_next();
    while(current_token->type != PARENTHESIS_CLOSE) {
        //TODO: Finish with function arguments
        consume_next();
    }

    if (func->args.size() == 0) {
        printf("empty function declaration");
    } else {

    }

    check_next(TokenType::FORWARD_ARROW);

    check_valid_return_type();

    DataType dt = data_type_map.at(current_token->lexeme);

    ReturnType* rt = new ReturnType(dt);
    func->return_type = rt;

    check_next(TokenType::COLON);

    Block* function_body = new Block();

    NodePTR ptr = NodePTR(NodeType::FUNCTION_DECLARATION, func);

    consume_block(ptr);

    check_next(TokenType::PASS);

    return func;

}

void Parser::consume_block(NodePTR& ptr) {

    switch(ptr.node_type) {
        case NodeType::FUNCTION_DECLARATION:
            FunctionDeclaration* func = static_cast<FunctionDeclaration*>(ptr.node_ptr);
            
        break;
    }
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