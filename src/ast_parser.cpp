#include "ast.hpp"

NodeRuleList Parser::parse(std::vector<std::unique_ptr<Token>>){
    return NodeRuleList(std::vector<std::unique_ptr<NodeRule>>());
}