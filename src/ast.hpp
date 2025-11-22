#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <utility>
#include <regex>
#include <memory> // std::unique_ptr

#include "token.hpp"

class Node;
class NodeRuleList;
class NodeRuleName;
class NodeRule;
class NodeAlternation;
class NodeConcatenation;
class NodeRepetition;
class NodeElement;
class NodeRepeat;
class NodeGroup;
class NodeOption;
class NodeCharVal;
class NodeNumVal;
class NodeBinVal;
class NodeDecVal;
class NodeHexVal;
class NodeProseVal;
class Parser;

class Node {
    public:
    virtual ~Node() = default;
    virtual std::string generate() = 0;
    virtual std::string to_string(size_t indent = 0) const = 0;
};

class NodeRuleList : public Node {
    public:
    std::vector<std::unique_ptr<NodeRule>> rules;
    inline NodeRuleList() = default;
    inline NodeRuleList(std::vector<std::unique_ptr<NodeRule>> rules) : rules(std::move(rules)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeRuleName : public Node {
    public:
    std::string name;
    inline NodeRuleName() = default;
    inline NodeRuleName(std::string name) : name(std::move(name)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeRule : public Node {
    public:
    std::unique_ptr<NodeRuleName> name;
    std::unique_ptr<NodeAlternation> alternation;
    inline NodeRule() = default;
    inline NodeRule(std::unique_ptr<NodeRuleName> name, std::unique_ptr<NodeAlternation> alternation) 
        : name(std::move(name)), alternation(std::move(alternation)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeAlternation : public Node {
    public:
    std::vector<std::unique_ptr<NodeConcatenation>> alternatives;
    inline NodeAlternation() = default;
    inline NodeAlternation(std::vector<std::unique_ptr<NodeConcatenation>> alternatives) 
        : alternatives(std::move(alternatives)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeConcatenation : public Node {
    public:
    std::vector<std::unique_ptr<NodeRepetition>> items;
    inline NodeConcatenation() = default;
    inline NodeConcatenation(std::vector<std::unique_ptr<NodeRepetition>> items) : items(std::move(items)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeRepetition : public Node {
    public:
    std::optional<std::unique_ptr<NodeRepeat>> repeat;
    std::unique_ptr<NodeElement> element;
    inline NodeRepetition() = default;
    inline NodeRepetition(std::optional<std::unique_ptr<NodeRepeat>> repeat, std::unique_ptr<NodeElement> element) 
        : repeat(std::move(repeat)), element(std::move(element)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeElement : public Node {
    public:
    std::variant<std::unique_ptr<NodeRuleName>, std::unique_ptr<NodeGroup>, std::unique_ptr<NodeOption>, 
                 std::unique_ptr<NodeCharVal>, std::unique_ptr<NodeNumVal>, std::unique_ptr<NodeProseVal>> element;
    inline NodeElement() = default;
    inline NodeElement(std::variant<std::unique_ptr<NodeRuleName>, std::unique_ptr<NodeGroup>, std::unique_ptr<NodeOption>, 
                                    std::unique_ptr<NodeCharVal>, std::unique_ptr<NodeNumVal>, std::unique_ptr<NodeProseVal>> element) 
        : element(std::move(element)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeRepeat : public Node {
    public:
    std::optional<size_t> min;
    std::optional<size_t> max;
    inline NodeRepeat() = default;
    inline NodeRepeat(std::optional<size_t> min, std::optional<size_t> max) 
        : min(std::move(min)), max(std::move(max)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeGroup : public Node {
    public:
    std::unique_ptr<NodeAlternation> alternation;
    inline NodeGroup() = default;
    inline NodeGroup(std::unique_ptr<NodeAlternation> alternation) : alternation(std::move(alternation)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeOption : public Node {
    public:
    std::unique_ptr<NodeAlternation> alternation;
    inline NodeOption() = default;
    inline NodeOption(std::unique_ptr<NodeAlternation> alternation) : alternation(std::move(alternation)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeCharVal : public Node {
    public:
    std::string contents;
    inline NodeCharVal() = default;
    inline NodeCharVal(std::string contents) : contents(std::move(contents)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeNumVal : public Node {
    public:
    std::variant<std::unique_ptr<NodeBinVal>, std::unique_ptr<NodeDecVal>, std::unique_ptr<NodeHexVal>> value;
    inline NodeNumVal() = default;
    inline NodeNumVal(std::variant<std::unique_ptr<NodeBinVal>, std::unique_ptr<NodeDecVal>, std::unique_ptr<NodeHexVal>> value) : value(std::move(value)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeBinVal : public Node {
    public:
    uint64_t base_num;
    std::optional<uint64_t> concat_num;
    std::optional<uint64_t> range_end;
    inline NodeBinVal() = default;
    inline NodeBinVal(uint64_t base_num, std::optional<uint64_t> concat_num = std::nullopt, 
                      std::optional<uint64_t> range_end = std::nullopt) 
        : base_num(base_num), concat_num(std::move(concat_num)), range_end(std::move(range_end)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeDecVal : public Node {
    public:
    uint64_t base_num;
    std::optional<uint64_t> concat_num;
    std::optional<uint64_t> range_end;
    inline NodeDecVal() = default;
    inline NodeDecVal(uint64_t base_num, std::optional<uint64_t> concat_num = std::nullopt, 
                      std::optional<uint64_t> range_end = std::nullopt) 
        : base_num(base_num), concat_num(std::move(concat_num)), range_end(std::move(range_end)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeHexVal : public Node {
    public:
    uint64_t base_num;
    std::optional<uint64_t> concat_num;
    std::optional<uint64_t> range_end;
    inline NodeHexVal() = default;
    inline NodeHexVal(uint64_t base_num, std::optional<uint64_t> concat_num = std::nullopt, 
                      std::optional<uint64_t> range_end = std::nullopt) 
        : base_num(base_num), concat_num(std::move(concat_num)), range_end(std::move(range_end)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class NodeProseVal : public Node {
    public:
    std::string contents;
    inline NodeProseVal() = default;
    inline NodeProseVal(std::string contents) : contents(std::move(contents)) {}
    std::string generate() override;
    std::string to_string(size_t indent = 0) const override;
};

class Parser {
    public:
    inline Parser(std::vector<std::unique_ptr<Token>> source) : source(std::move(source)) {}
    ~Parser();

    static NodeRuleList parse(std::vector<std::unique_ptr<Token>> source);
    private:
    std::vector<std::unique_ptr<Token>> source;
};

std::ostream & operator<< (std::ostream & os, NodeRuleList list);
