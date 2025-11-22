#pragma once

#include <string>
#include <vector>
#include <variant>

#include "token.hpp"

class NodeBase {
    virtual std::string render() = 0;
};

class NodeHeading : public NodeBase {
    public:
    NodeHeading();
    NodeHeading(TokenHeading contents);
    ~NodeHeading() = default;
    TokenHeading contents;
};

class NodeParagraph : public NodeBase {
    public:
    NodeParagraph();
    NodeParagraph(TokenStringLiteral);
    NodeParagraph(std::vector<TokenStringLiteral>);
    ~NodeParagraph() = default;
    std::vector<TokenStringLiteral> children;
};

class NodeDocument : public NodeBase {
    public:
    NodeDocument() = default;
    ~NodeDocument() = default;
    std::vector<std::variant<NodeHeading, NodeParagraph>> children;
};

class Parser {
    public:
    inline Parser(std::vector<std::unique_ptr<Token>>) : source(source) {};
    ~Parser();

    NodeDocument parse();
    private:
    std::vector<std::unique_ptr<Token>> source;
};
