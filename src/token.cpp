#include "token.hpp"

#include <format>

Token::Token(TokenType type, const std::string & contents)
    : type(type), contents(contents) {}

std::ostream & operator<<(std::ostream &os, const Token token){
    std::string this_str = token.to_string();
    if(token.type == newline) this_str += "\n";
    return os << this_str;
}

std::ostream & operator<< (std::ostream & os, std::vector<std::unique_ptr<Token>> tokens){
    std::vector<std::string> v;
    for (size_t i = 0; i < tokens.size(); i++){
        v.push_back(tokens[i]->to_string());
    }
    os << join(v, ' ');
    return os;
}

Tokenizer::Tokenizer(const std::string & src, const std::string & filename)
    : source(std::move(src)), filename(filename), index(0), eof(false) {}

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

[[nodiscard]] std::string Tokenizer::current_line_index(){
    size_t index = this->index;
    // Without chaning the actual index, skip preceding newlines
    while(source.at(index) == '\n' || source.at(index) == '\r') index++;

    size_t prev_newline = source.substr(0,index).find_last_of('\n');
    if(prev_newline == std::string::npos) prev_newline = 0;

    size_t next_newline = source.substr(index).find_first_of('\n');
    if(next_newline == std::string::npos) next_newline = source.size();

    // If prev_newline is 0, there isn't a newline to drop from this_line
    if(prev_newline != 0) prev_newline++;
    std::string this_line = source.substr(prev_newline, next_newline);
    size_t cursor = index - prev_newline;
    // Drop leading whitespace chars
    size_t to_drop = 0;
    for(auto &c : this_line.substr(0,cursor)){
        if(c == '\r' || c == ' ' || c == '\t') to_drop ++;
        else break;
    }
    cursor -= to_drop;
    this_line = this_line.substr(to_drop);
    // Drop any trailing CR
    if(this_line.at(this_line.size() - 1) == '\r') this_line.pop_back();

    size_t line_num = std::ranges::count(source.substr(0,index), '\n');

    std::stringstream ss;
    ss << filename << ":" << line_num << ":" << cursor << "\n";
    ss << this_line << "\n";
    while(cursor--) ss << " ";
    ss << "^";
    return ss.str();
}

[[nodiscard]] std::vector<std::unique_ptr<Token>> Tokenizer::tokenize(std::string source, std::string filename) {
    Tokenizer t(source, filename);
    std::vector<std::unique_ptr<Token>> ret;

    while (!t.eof){
        std::smatch m;
        std::string rem = t.remaining();

        std::regex_search(rem, m, TokenComment::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenComment>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenIdentifier::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenIdentifier>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenDefineInc::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenDefineInc>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenDefine::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenDefine>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenSlash::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenSlash>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenLParen::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenLParen>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenRParen::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenRParen>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenLBrack::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenLBrack>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenRBrack::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenRBrack>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenStar::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenStar>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenStringLiteral::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenStringLiteral>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenIntegerLiteral::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenIntegerLiteral>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenNumValue::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenIntegerLiteral>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 

        std::regex_search(rem, m, TokenProseLiteral::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenProseLiteral>(match));
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 





        std::regex_search(rem, m, TokenNewline::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenNewline>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 
        
        std::regex_search(rem, m, TokenWhitespace::regex);
        for(auto r: m){
            std::string match = r.str();
            ret.push_back(std::make_unique<TokenWhitespace>());
            t.pop(match.length());
            // First sub-match only
            break;
        }
        if(m.length() > 0) continue; 
        
        std::cerr << "Unknown token!" << std::endl << t.current_line_index() << std::endl;
        return ret;
    }
    
    return ret;
}