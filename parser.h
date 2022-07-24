#pragma once

#include <memory>
#include <vector>
#include "token.h"
#include "expr.h"

class Parser {
    public:
        Parser(const std::vector<Token>& tokens)
        : tokens{tokens} {}

        std::unique_ptr<Expr> parse();

    private:
        bool atEnd();
        Token peek();
        Token advance();
        Token previous();
        bool check(TokenType type);
        bool match(TokenType type);

        std::unique_ptr<Expr> term();
        std::unique_ptr<Expr> factor();
        std::unique_ptr<Expr> unary();
        std::unique_ptr<Expr> primary(); 

    private:
        std::vector<Token> tokens;
        size_t current = 0;
};
