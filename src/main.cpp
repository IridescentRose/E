#include <iostream>
#include <vector>
#include <string>
#include "util.hpp"

enum TokenType {
        Eof = -1,
        Number = 1,
        Identifier = 2,
};

struct Token {
        TokenType type;
        size_t line, column;

        double number = 0;
        std::string identifier = "";

        Token(TokenType type, size_t line, size_t column) : type(type), line(line), column(column) {}
};

auto tokenize(std::string buffer) -> std::vector<Token> {
        std::vector<Token> tokens;

        size_t currentLine = 0;
        size_t currentColumn = 0;

        std::string currentToken = "";

        auto addToken = [&]() {
                if(currentToken == "") return;

                Token tok(TokenType::Identifier, currentLine, currentColumn);
                tok.identifier = currentToken;
                tokens.push_back(tok);
                currentToken = "";
        };

        for(auto i = 0; i < buffer.length(); i++) {
                currentColumn++;

                auto c = buffer[i];

                if(c == '\n') {
                        addToken();
                        currentLine++;
                        currentColumn = 0;
                } else if(c == ' ') {
                        addToken();
                        currentColumn++;
                } else if(c == '\t') {
                        addToken();
                        currentColumn += 4;
                } else {
                        currentToken += c;
                }
        }
        Token eofTok(TokenType::Eof, currentLine, currentColumn);
        tokens.push_back(eofTok);

        return tokens;
}

auto main (int argc, char *argv[]) -> int 
{
        if(argc < 2) {
                std::cerr << "Error: No file provided!\n" << "Usage: ec <input_file.c>\n" << std::endl;
                return EXIT_FAILURE;
        }

        auto fb = readFileBuf(argv[1]);
        auto tokens = tokenize(fb);

        for(auto tok : tokens) {

                std::cout << "Token: ";
                std::cout << tok.type << " ";
                std::cout << tok.line << ":" << tok.column << " ";

                if(tok.type == TokenType::Identifier)
                        std::cout << tok.identifier << std::endl;
                else 
                        std::cout << std::endl;
        }

        return EXIT_SUCCESS;
}
