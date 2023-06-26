#pragma once
#include <vector>
#include <string>
#include <stdexcept>

enum TokenType {
        Eof = -1,
        Number = 1,
        Identifier = 2,
        Semicolon = 3,
        Preprocessor = 4,
        LeftBrace = 5,
        RightBrace = 6,
        LeftParen = 7,
        RightParen = 8,
        StringLiteral = 9,
};

struct Token {
        TokenType type = TokenType::Eof;
        size_t line = 0, column = 0;

        double number = 0;
        std::string identifier = "";

        Token(TokenType type, size_t line, size_t column) : type(type), line(line), column(column) {}
};

class Tokenizer {
public:
        Tokenizer(std::vector<Token>& tokens) : tokens(tokens) {
                tokens.clear();
                tokens.shrink_to_fit();
        
                current_line = 1;
                current_column = 0;
        }

        auto tokenize(std::string buffer) -> void;
private:
        std::vector<Token>& tokens;
        std::string current_token; 
        size_t current_line;
        size_t current_column;

        auto add_token() -> void;
        auto add_special(TokenType type, std::string identifier) -> void;
        auto add_string_literal(size_t& i, std::string& buffer) -> void;
        auto handle_special(char c) -> bool;
};