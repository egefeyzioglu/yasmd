#pragma once

#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <memory> // std::unique_ptr
#include <algorithm>


enum TokenType {
    comment,
    identifier,
    define_inc,
    define,
    slash,
    lparen,
    rparen,
    lbrack,
    rbrack,
    star,
    string_literal,
    num_value,
    prose_literal,
    integer_literal,
    newline,
    whitespace
};

class Token{
    public:
    Token(TokenType type, const std::string & contents);
    TokenType type;
    std::string contents;

    inline virtual std::regex pattern() const {return std::regex("");}

    inline virtual std::string to_string() const {return "[Unknown]";};
    friend std::ostream & operator<<(std::ostream &os, const Token token);
};

class Tokenizer {
    public:
    Tokenizer();
    Tokenizer(const std::string & source, const std::string & filename);

    [[nodiscard]] static std::vector<std::unique_ptr<Token>> tokenize(std::string source, std::string filename);

    [[nodiscard]] char peek(size_t offset = 0) const;
    char pop(size_t num = 1);
    std::string remaining() const;

    bool is_eof() const;

    std::string current_line_index();

    private:
    std::string source;
    std::string filename;
    size_t index;
    bool eof;
};

////////////////////////////////

class TokenComment : public Token {
    public:
    inline TokenComment(std::string contents) : Token(comment, contents) {};
    virtual inline std::string to_string() const {
        return "[Comment contents=\"" + contents + "\"]";
    }
    static inline const std::regex regex = std::regex("^;.*");
};

class TokenIdentifier : public Token {
    public:
    inline TokenIdentifier(std::string name) : Token(identifier, name) {}
    virtual inline std::string to_string() const {
        return "[Identifier name=\"" + contents + "\"]";
    }
    static inline const std::regex regex = std::regex("^[a-zA-Z][a-zA-Z\\-0-9]*");
};

class TokenDefineInc : public Token {
    public:
    inline TokenDefineInc() : Token(define_inc, "") {}
    virtual inline std::string to_string() const {
        return "[DefineInc]";
    }
    static inline const std::regex regex = std::regex("^=/");
};

class TokenDefine : public Token {
    public:
    inline TokenDefine() : Token(define, "") {}
    virtual inline std::string to_string() const {
        return "[Define]";
    }
    static inline const std::regex regex = std::regex("^=");
};

class TokenSlash : public Token {
    public:
    inline TokenSlash() : Token(slash, "") {}
    virtual inline std::string to_string() const {
        return "[Slash]";
    }
    static inline const std::regex regex = std::regex("^/");
};

class TokenRParen : public Token {
    public:
    inline TokenRParen() : Token(rparen, "") {}
    virtual inline std::string to_string() const {
        return "[RParen]";
    }
    static inline const std::regex regex = std::regex("^\\)");
};

class TokenLParen : public Token {
    public:
    inline TokenLParen() : Token(lparen, "") {}
    virtual inline std::string to_string() const {
        return "[LParen]";
    }
    static inline const std::regex regex = std::regex("^\\(");
};

class TokenRBrack : public Token {
    public:
    inline TokenRBrack() : Token(rbrack, "") {}
    virtual inline std::string to_string() const {
        return "[RBrack]";
    }
    static inline const std::regex regex = std::regex("^\\]");
};

class TokenLBrack : public Token {
    public:
    inline TokenLBrack() : Token(lbrack, "") {}
    virtual inline std::string to_string() const {
        return "[LBrack]";
    }
    static inline const std::regex regex = std::regex("^\\[");
};

class TokenStar : public Token {
    public:
    inline TokenStar() : Token(star, "") {}
    virtual inline std::string to_string() const {
        return "[Star]";
    }
    static inline const std::regex regex = std::regex("^\\*");
};

class TokenStringLiteral : public Token {
    public:
    inline TokenStringLiteral(std::string contents) : Token(string_literal, contents) {}
    virtual inline std::string to_string() const {
        return "[StringLiteral contents=\"" + contents + "\"]";
    }
    static inline const std::regex regex = std::regex("^\"[\\x20-\\x22\\x23-\\x7e]*\"");
};

template <typename T>
static std::string join(std::vector<T> v, char join = ','){
    if(v.empty()) return "";
    std::stringstream ss;
    for(auto item = v.begin(); item != v.end() - 1; item++){
        ss << *item << join;
    }
    ss << v[v.size() - 1];
    return ss.str();
}

class TokenNumValue : public Token {
    public:
    inline TokenNumValue(std::string contents, std::vector<std::string> ranges) : Token(num_value, contents), base(contents[0]), ranges(ranges) {}
    virtual inline std::string to_string() const {
        std::stringstream ss;
        ss << "[NumValue contents=\"" << contents << "\"]";
        return ss.str();
    }
    static inline const std::regex regex = std::regex("^%([bdx])([0-9a-fA-F]+)(([\\-.]?)([0-9a-fA-F]*))*");
    char base;
    std::vector<std::string> ranges;
};

class TokenProseLiteral : public Token {
    public:
    inline TokenProseLiteral(std::string contents) : Token(prose_literal, contents) {}
    virtual inline std::string to_string() const {
        return "[ProseLiteral contents=\"" + contents + "\"]";
    }
    static inline const std::regex regex = std::regex("^<[\\x20-\\x3D\\x3F-\\x7E]*>");
};

class TokenIntegerLiteral : public Token {
    public:
    inline TokenIntegerLiteral(std::string contents) : Token(integer_literal, contents) {}
    virtual inline std::string to_string() const {
        return "[IntegerLiteral contents=\"" + contents + "\"]";
    }
    static inline const std::regex regex = std::regex("^[0-9]+");
};

class TokenNewline : public Token {
    public:
    inline TokenNewline() : Token(newline, "") {}
    virtual inline std::string to_string() const {
        return "[NL]";
    }
    static inline const std::regex regex = std::regex("^\\n|\\r|\\r\\n");
};

class TokenWhitespace : public Token {
    public:
    inline TokenWhitespace() : Token(whitespace, "") {}
    virtual inline std::string to_string() const {
        return "[WS]";
    }
    static inline const std::regex regex = std::regex(" |\\t");
};

////////////////////////////////

std::ostream & operator<< (std::ostream & os, std::vector<std::unique_ptr<Token>>);