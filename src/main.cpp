#include <iostream>
#include "util.hpp"
#include "Tokenizer.hpp"

auto main (int argc, char *argv[]) -> int 
{
        if(argc < 2) {
                std::cerr << "Error: No file provided!\n" << "Usage: ec <input_file.c>\n" << std::endl;
                return EXIT_FAILURE;
        }

        auto fb = readFileBuf(argv[1]);

        std::vector<Token> tokens;
        Tokenizer tokenizer(tokens);
        
        try {
                tokenizer.tokenize(fb);
        } catch(std::runtime_error& e) {
                std::cerr << "Error in tokenization!\n" << e.what() << std::endl;
                return EXIT_FAILURE;
        }

        // TODO: Remove this
        for(auto tok : tokens) {

                std::cout << "Token: ";
                std::cout << tok.type << " ";
                std::cout << tok.line << ":" << tok.column << " ";

                if(tok.type != TokenType::Eof)
                        std::cout << tok.identifier << std::endl;
                else 
                        std::cout << std::endl;
        }

        return EXIT_SUCCESS;
}
