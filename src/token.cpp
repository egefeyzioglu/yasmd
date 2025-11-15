#include "token.hpp"

Token::Token(TokenType type, const std::string & contents)
    : type(type), contents(contents) {}


std::string TokenHeading::to_string() const {
    std::stringstream ss;
    ss << "[Heading level=" << level() << " contents=\"" << contents << "\"]";
    return ss.str();
}

std::string TokenStringLiteral::to_string() const {
    std::stringstream ss;
    ss << "[String contents=\"" << contents << "\"]";
    return ss.str();
}

std::string TokenSpace::to_string() const {
    return "[Space]";
}

std::ostream & operator<<(std::ostream &os, const Token token){
    return os << token.to_string();
}

std::ostream & operator<< (std::ostream & os, std::vector<std::unique_ptr<Token>> tokens){
    std::stringstream ss;
    for (size_t i = 0; i < tokens.size(); i++){
        ss << tokens[i]->to_string() << " ";
    }
    os << ss.str();
    return os;
}

Tokenizer::Tokenizer(const std::string & src)
    : source(std::move(src)), index(0), eof(false) {}

Tokenizer::Tokenizer()
    : source(""), index(0), eof(false) {}

char Tokenizer::pop(size_t num){
    if(index + num >= source.length()){
        eof = true;
        return 0;
    }
    index += num;
    return source.at(index - num);
}

[[nodiscard]] char Tokenizer::peek(size_t offset) const{
    return source.at(index + offset);
}

[[nodiscard]] std::string Tokenizer::remaining() const{
    return std::string(source.begin() + index, source.end());
}

[[nodiscard]] std::vector<std::unique_ptr<Token>> Tokenizer::tokenize(std::string source) {
    Tokenizer t(source);
    std::vector<std::unique_ptr<Token>> ret;

    while (!t.eof){
        std::smatch m;
        std::string rem = t.remaining();

        // TODO: Make this not hardcoded/in both places
        std::regex_search(rem, m, std::regex("^#+.*"));
        for(auto r: m){
            std::string match = r.str();
            t.pop(match.length());
            int level = 0;
            for(char c: match){
                if(c == '#') level++;
                else break;
            }
            ret.push_back(std::make_unique<TokenHeading>(match, level));
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 
        
        // TODO: Make this not hardcoded/in both places
        std::regex_search(rem, m, std::regex("^[^\\s#][^\\n\\r]*"));
        for(auto r: m){
            std::string match = r.str();
            t.pop(match.length());
            ret.push_back(std::make_unique<TokenStringLiteral>(match));
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 
        
        // Drop spaces, tabs, and newlines
        bool trimmed_space = false;
        bool yeet = true;
        for(size_t i = 0; yeet && i < rem.length(); i++){
            switch (rem.at(i))
            {
            case '\n':
            case '\r':
            case '\t':
            case ' ':
                t.pop();
                trimmed_space = true;
                break;
            
            default:
                yeet = false;
                break;
            }
        }
        if(trimmed_space) continue;
        
        std::cerr << "Unknown token starting from: " << rem << std::endl;
        break;
    }
    
    return ret;
}