#include "rules.h"
#include "parser.h"
#include <iostream>

RulesManager::RulesManager() {

    ruleset = std::map<TokenType, ParserData>();

    // Rule: Class Definition

    Rule rule_class_definition {
        AtomicRule(     TokenType::CLASS_NAME   ),
        AtomicRule(     TokenType::IDENTIFIER   )
    };

    ParserData data_class_definition(rule_class_definition,NodeType::CLASS_DECLARATION);

    ruleset[TokenType::CLASS_NAME] = data_class_definition;

    // //======================================

    // // Rule: Inheritance
    
    // Rule rule_inheritance {
    //     AtomicRule(     TokenType::EXTENDS      ),
    //     AtomicRule(     TokenType::IDENTIFIER   )
    // };

    // ruleset[TokenType::EXTENDS] = rule_inheritance;

    // //======================================

    // // Rule: Function Declaration
    
    // Rule rule_function_declaration {
    //     AtomicRule(     TokenType::FUNC                 ),
    //     AtomicRule(     TokenType::IDENTIFIER           ),
    //     AtomicRule(     AtomicRule::R_SubRule           ), //Grouping
    //     AtomicRule(     TokenType::FORWARD_ARROW        ),
    //     AtomicRule(     TokenType::KEYWORD              ),


    // };

    // ruleset[TokenType::FUNC] = rule_function_declaration;

    // //======================================

};

RulesManager::~RulesManager() {

};

ParserData RulesManager::find_rule(const TokenVector& tokens, int& index) {
    Token token = tokens[index];
    std::cout << "Finding rule for token: " << STR_TOKEN_TYPE[token.type] << "\n";

    auto t = ruleset.find(token.type);

    if (t == ruleset.end()) {

        printf("rule not found!");

        //TODO: Make error push
        // return ParserData(Rule());
    }

    Rule rule = t->second.rule;
    NodeType node_type = t->second.node_type;
    Node* node_ptr;
    // switch(node_type) {
    //     case CLASS_DECLARATION:
    //         node_ptr = new ClassDeclaration()
    //     break;
    // } 

    for (size_t i = 0; i < rule.size(); i++)
    {
        AtomicRule* a_rule = &rule[i];

        switch(a_rule->rule_type)
        {
            case AtomicRule::R_TokenType:

                if (token.type == a_rule->token_type){
                    index++;
                    token = tokens[index];
                    continue;
                }


            break;

            case AtomicRule::R_OR:

            break;

            case AtomicRule::R_SubRule:

            break;
        }
    }

    // Node n = t->second.node;
    // n = ClassDeclaration("");
    // return n;
    
    //TODO: remove me!
    return ruleset[TokenType::CLASS_NAME];
};

