#ifndef GD_PARSER
#define GD_PARSER

#include <vector>
#include <memory>

#include "lexer.h"

enum NodeType {
    NODE, //NON USABLE
    SCRIPT,
    INHERITANCE,
    CLASS_DECLARATION,
    FUNCTION_DECLARATION,
    FUNCTION_CALL,
    ARGUMENT,
    LITERAL,
    BLOCK,
    GROUPING,
    RETURN_TYPE,
    NODE_IDENTIFIER
};

struct Node;
struct Script;
struct ClassDeclaration;
struct FunctionDeclaration;
struct FunctionCall;
struct Inheritance;
struct Literal;
struct Block;
struct Grouping;
struct ReturnType;
struct Parameter;

struct NodePTR; //This struct is responsible for recognizing the Node* real type

struct Node {
    NodeType type;
    Token* token;

    Node(NodeType t) : type(NODE) {};
};

struct Block : Node {
    std::vector<NodePTR> nodes;

    Block() : Node(BLOCK) {
        nodes = std::vector<NodePTR>();
    };
};

struct Script : Node {
    Block* body;
    Inheritance* parent;
    ClassDeclaration* class_declaration;

    Script() : Node(SCRIPT) {};
};

struct Literal : Node {
    std::string value;

    Literal() : Node(LITERAL) {};
};

struct Identifier : Node {
    std::string value;

    Identifier() : Node(NODE_IDENTIFIER) {};
};

struct ReturnType : Node {
    DataType data_type;
    ReturnType(DataType dt) : Node(RETURN_TYPE), data_type(dt) {};
};

struct Grouping : Node {


    Grouping() : Node(GROUPING) {};
};

struct Inheritance : Node {
    std::string parent_name;

    Inheritance(std::string pn) : Node(INHERITANCE), parent_name(pn) {};
};

struct ClassDeclaration : Node {
    std::string class_name;

    ClassDeclaration(std::string cn) : Node(CLASS_DECLARATION), class_name(cn) {};
};

struct FunctionDeclaration : Node {
    std::string function_name;
    ReturnType* return_type;
    std::vector<Parameter*> args;
    std::vector<NodePTR> body;
    FunctionDeclaration(std::string fn) : Node(FUNCTION_DECLARATION) {};
};

struct Parameter : Node {
    std::string value;
    DataType data_type;

    Parameter(std::string v) : Node(ARGUMENT), value(v) {};
};

struct FunctionCall : Node {

    std::string function_name;
    std::vector<Parameter*> args;

    FunctionCall() : Node(FUNCTION_CALL) {};
};

struct Error {
    std::string msg;

    Error(Token& token, std::string message) {
        msg = "At: " + std::to_string(token.line) + " : " + std::to_string(token.column) + " -> " + message +"\n";
    };
};

struct NodePTR {
    NodeType node_type;
    Node* node_ptr;

    NodePTR(NodeType nt, Node* ptr) : node_type(nt), node_ptr(ptr) {}; 
};

class Parser {

    public:
        Parser();
        void parse(TokenVector& tokens);

    private:
        int index;
        TokenVector* tokens;
        Token* current_token;
        void create_nodes(const NodePTR& p_owner,Block& p_block);
        void create_script(Script& scr);
        void consume_next();
        void check_next(TokenType token_type);
        void check_valid_return_type();
        
        // std::vector<Error> errors;
        Error* error;
        void push_error();

        //Node Creation functions
        ClassDeclaration* create_node_class_declaration();
        Inheritance* create_inheritance();
        FunctionDeclaration* create_function();
        


};


#endif // GD_PARSER