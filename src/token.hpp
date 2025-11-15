#pragma once

#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <memory> // std::unique_ptr


enum TokenType {
    string_literal,
    heading,
    space
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
    Tokenizer(const std::string & source);

    [[nodiscard]] static std::vector<std::unique_ptr<Token>> tokenize(std::string source);

    [[nodiscard]] char peek(size_t offset = 0) const;
    char pop(size_t num = 1);
    std::string remaining() const;

    bool is_eof() const;

    private:
    std::string source;
    size_t index;
    bool eof;
};

class TokenSpace : public Token {
    public:
    TokenSpace() = delete;
    virtual inline std::regex pattern() const {
        return std::regex("^\\s+");
    }
    virtual std::string to_string() const;
};

class TokenHeading : public Token {
    public:
    inline TokenHeading(const std::string &contents, int level)
        : Token(TokenType::heading, contents), _level(level) {}
    inline int level() const {return _level;}
    virtual inline std::regex pattern() const {
        return std::regex("^#+.*");
    }

    virtual std::string to_string() const;
    
    private:
    int _level;
};

class TokenStringLiteral : public Token {
    public:
    inline TokenStringLiteral(const std::string &contents) : Token(TokenType::string_literal, contents){}   
    virtual std::string to_string() const;
    virtual inline std::regex pattern() const {
        return std::regex("^[^\\s#][^\\n\\r]*");
    }
};

std::ostream & operator<< (std::ostream & os, std::vector<std::unique_ptr<Token>>);