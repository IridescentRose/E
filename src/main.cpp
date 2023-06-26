#include <iostream>
#include "util.hpp"
#include "Tokenizer.hpp"

class ExprAST {
public:
        virtual ~ExprAst() = default;

};

class PrototypeAST : public ExprAST {
        std::string name;
        std::vector<std::string> args;

public:
        PrototypeAST(const std::string& Name, const std::vector<std::string> Args) 
                : name(Name), args(std::move(Args)) {}
        const std::string& getName() { return name; }
};

class CallExprAst : public ExprAST {
        std::string callee;
        std::vector<std::unique_ptr<ExprAST>> args;

public:
        CallExprAst(const std::string& Callee, std::vector<std::unique_ptr<ExprAST> Args)
                : callee(Callee), args(std::move(Args)) {}
};

class FunctionAST : public ExprAST {
        std::unique_ptr<PrototypeAST> prototype;
        std::unique_ptr<ExprAST> body;

public:
        FunctionAST(std::unique_ptr<PrototypeAST> Prototype, std::unique_ptr<ExprAst> Body)
                : prototype(std::move(Prototype)), body(std::move(Body)) {}
};



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
