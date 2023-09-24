#ifndef GD_RULES
#define GD_RULES

#include "lexer.h"
#include "parser.h"
#include <vector>

struct AtomicRule {

    enum RuleType {
        R_TokenType,
        R_SubRule,
        R_OR,
    };

    RuleType rule_type;
    TokenType token_type;

    AtomicRule(RuleType t) : rule_type(t), token_type(TK_MAX) {};
    AtomicRule(TokenType tt) : rule_type(R_TokenType), token_type(tt) {};

};

typedef std::vector<AtomicRule> Rule;

struct ParserData {
    Rule rule;
    Node* node_ptr;
    NodeType node_type;

    // ParserData(Rule rule, Node* n) : rule(rule), node_ptr(n) {};
    ParserData(Rule rule, NodeType nt) : rule(rule), node_ptr(nullptr), node_type(nt) {};
    ParserData() : rule({}), node_ptr(nullptr), node_type(NodeType::ARGUMENT) {};
};


class RulesManager {

    public:
        RulesManager();
        ~RulesManager();
        ParserData find_rule(const TokenVector& tokens, int& index);

    private:
        std::map<TokenType, ParserData> ruleset;

};


#endif