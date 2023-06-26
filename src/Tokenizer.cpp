#include "Tokenizer.hpp"

auto Tokenizer::add_token() ->  void {
        if(current_token == "") return;

        Token tok(TokenType::Identifier, current_line, current_column);
        tok.identifier = current_token;
        tok.column -= current_token.length();
        tokens.push_back(tok);
        current_token = "";
};

auto Tokenizer::add_special(TokenType type, std::string identifier) -> void {                
        add_token();
        Token tok(type, current_line, current_column);
        tok.identifier = identifier;
        tok.column -= identifier.length();
        tokens.push_back(tok);
};


auto Tokenizer::add_string_literal(size_t& i, std::string& buffer) -> void {
        current_token = "";
        Token tok(TokenType::StringLiteral, current_line, current_column);
        tok.identifier = "\"";

        while(true) {
                i++;
                current_column++;

                if(buffer[i] == '"') {
                        tok.identifier += "\"";
                        break;
                } else if(buffer[i] == '\n') {
                        throw std::runtime_error("Error: Unterminated string literal!");
                } else {
                        tok.identifier += buffer[i];
                }
        }

        tokens.push_back(tok);
}

auto Tokenizer::handle_special(char c) -> bool {
        switch(c) {
                case ';': {
                        add_special(TokenType::Semicolon, ";");
                        return true;
                }

                case '{': {
                        add_special(TokenType::LeftBrace, "{");
                        return true;
                }

                case '}': {
                        add_special(TokenType::RightBrace, "}");
                        return true;
                }

                case '(': {
                        add_special(TokenType::LeftParen, "(");
                        return true;
                }

                case ')': {
                        add_special(TokenType::RightParen, ")");
                        return true;
                }

                default: {
                        return false;
                }
        }
}

auto Tokenizer::tokenize(std::string buffer) -> void {
        // TODO: Actually handle preprocessor directives properly
        // If we choose to support them, that is
        bool preprocessor = false;
        for(size_t i = 0; i < buffer.length(); i++) {
                current_column++; // Sets the column to 1 on the first character of the line -- this is intentional

                auto c = buffer[i];

                if(preprocessor) {
                        if(c == '\n') { // A preprocessor directive can only be one line long
                                preprocessor = false;
                                Token tok(TokenType::Preprocessor, current_line, current_column);
                                tok.identifier = current_token;
                                tok.column -= current_token.length();
                                tokens.push_back(tok);
                                current_token = "";
                        } else { // We're still in the preprocessor directive
                                current_token += c;
                                continue;
                        }
                }

                if(c == '\n') {
                        add_token();
                        current_line++;
                        current_column = 0;
                } else if(c == ' ') {
                        add_token();
                        current_column++;
                } else if(c == '\t') {
                        add_token();
                        current_column += 4;
                } else {
                        if(c == '#' && current_column != 1) {
                                throw std::runtime_error("Error: " + std::to_string(current_line) +":1 Preprocessor directive not at the start of the line!");
                        } else if(c == '#' && current_token == "") {
                                preprocessor = true;
                                current_token += c;
                                continue;
                        }

                        if(c == '"') {
                                add_string_literal(i, buffer);
                                continue;
                        }

                        // TODO: There's an argument to be made that we should keep commas when transpiling to C
                        if(c == '/' && buffer[i + 1] == '/') {
                                while(buffer[i] != '\n') { i++; } // Skip to the end of the line
                                current_line++; // We're on the next line now
                                current_column = 0;
                                continue;
                        }

                        if(!handle_special(c))
                                current_token += c;
                }
        }
        Token eofTok(TokenType::Eof, current_line, current_column);
        tokens.push_back(eofTok);
}