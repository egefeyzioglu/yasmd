#include "ast.hpp"

#define DO_INDENT() do{do_indent(ss,indent);}while(0)

std::ostream & operator<<(std::ostream & os, NodeRuleList list) {
    return os << list.to_string();
}

inline void do_indent(std::stringstream & ss, size_t indent, char indentchar = ' '){
    while(indent--) ss << indentchar;
}

std::string NodeRuleList::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Rule List(" << rules.size() << ")\n";
    for(const auto& rule : rules){
        ss << rule->to_string(indent + 2);
    }
    return ss.str();
}

std::string NodeRuleName::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Rule Name\n";
    DO_INDENT();
    ss << "Name:" << name << "\n";
    return ss.str();
}

std::string NodeRule::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Rule\n";
    DO_INDENT();
    ss << "Name:\n";
    ss << name->to_string(indent + 2);
    return ss.str();
}

std::string NodeAlternation::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Alternation\n";
    DO_INDENT();
    for(const auto& concat : alternatives){
        ss << concat->to_string(indent + 2);
    }
    return ss.str();
}

std::string NodeConcatenation::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Concatenation\n";
    for(const auto& rep : items){
        ss << rep->to_string(indent + 2);
    }
    return ss.str();
}

std::string NodeRepetition::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Repetition\nRepeat:";
    if(repeat.has_value()){
        ss << repeat.value()->to_string(indent + 2);
    }else{
        ss << "[No Value]";
    }
    ss << "\n";
    ss << element->to_string(indent + 2);
    return ss.str();
}

std::string NodeElement::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Element\n";
    
    // TODO: I'm told std::visit is better for this
    if(auto *elem = std::get_if<std::unique_ptr<NodeRuleName>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(auto *elem = std::get_if<std::unique_ptr<NodeGroup>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(auto *elem = std::get_if<std::unique_ptr<NodeOption>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(auto *elem = std::get_if<std::unique_ptr<NodeCharVal>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(auto *elem = std::get_if<std::unique_ptr<NodeNumVal>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(auto *elem = std::get_if<std::unique_ptr<NodeProseVal>>(&element)){
        ss << (*elem)->to_string(indent + 2);
    } else if(element.valueless_by_exception()) {
        DO_INDENT();
        ss << "valueless_by_exception in NodeElement@" << this << "\n";
    }else {
        DO_INDENT();
        ss << "Unknown element in NodeElement@" << this << "\n";
    }

    return ss.str();
}

std::string NodeRepeat::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Repeat: ";
    ss << min.has_value() ? std::to_string(min.value()) : "[0]";
    ss << "*";
    ss << max.has_value() ? std::to_string(max.value()) : "inf";

    return ss.str();
}

std::string NodeGroup::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Group\n";
    DO_INDENT();
    ss << "Alternation:\n";
    ss << alternation->to_string(indent + 2);
    return ss.str();
}

std::string NodeOption::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "Option\n";
    DO_INDENT();
    ss << "Alternation:\n";
    ss << alternation->to_string(indent + 2);
    return ss.str();
}

std::string NodeCharVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "CharVal: \"" << contents << "\"\n";
    return ss.str();
}

std::string NodeNumVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "NodeNumVal:\n";

    // TODO: I'm told std::visit is better for this
    if(auto *val = std::get_if<std::unique_ptr<NodeBinVal>>(&value)){
        ss << (*val)->to_string(indent + 2);
    } else if(auto *val = std::get_if<std::unique_ptr<NodeDecVal>>(&value)){
        ss << (*val)->to_string(indent + 2);
    } else if(auto *val = std::get_if<std::unique_ptr<NodeHexVal>>(&value)){
        ss << (*val)->to_string(indent + 2);
    } else if(value.valueless_by_exception()) {
        DO_INDENT();
        ss << "valueless_by_exception in NodeNumVale@" << this << "\n";
    }else {
        DO_INDENT();
        ss << "Unknown element in NodeNumVal@" << this << "\n";
    }

    return ss.str();
}

static std::string uint64tohex(uint64_t i){
    if(i == 0) return "0";
    std::string ret = "";
    bool seen_one = false;
    for(ssize_t shift = 60; shift >= 0; shift -= 4){
        uint8_t this_nibble = (i >> shift) & 0xf;
        seen_one |= (this_nibble != 0);
        if(seen_one){
            ret += this_nibble < 10 ? '0' + this_nibble : 'a' + this_nibble - 10;
        }
    }
    return ret;
}

static std::string uint64tobin(uint64_t i){
    if(i == 0) return "0";
    std::string ret = "";
    ret.reserve(64);
    bool seen_one = false;
    uint64_t mask = uint64_t(0b1) << 63;
    while(mask){
        seen_one |= (i & mask) != 0;
        if(seen_one) ret += (i & mask) ? "1" : "0";
        mask = mask >> 1;
    }
    return ret;
}

std::string NodeBinVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "NodeBinVal: 0b" << uint64tobin(base_num);
    if(concat_num.has_value()){
        ss << "." << concat_num.value();
    } else if(range_end.has_value()){
        ss << "-" << range_end.value();
    }
    return ss.str();
}

std::string NodeDecVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "NodeBinVal: " << std::to_string(base_num);
    if(concat_num.has_value()){
        ss << "." << concat_num.value();
    } else if(range_end.has_value()){
        ss << "-" << range_end.value();
    }
    return ss.str();
}

std::string NodeHexVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "NodeBinVal: 0x" << uint64tohex(base_num);
    if(concat_num.has_value()){
        ss << "." << concat_num.value();
    } else if(range_end.has_value()){
        ss << "-" << range_end.value();
    }
    return ss.str();
}

std::string NodeProseVal::to_string(size_t indent) const {
    std::stringstream ss;
    DO_INDENT();
    ss << "NodeProseVal: \"" << contents << "\"";
    return ss.str();
}