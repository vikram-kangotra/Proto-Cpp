#include "parser.h"
#include <exception>
#include <iostream>

// expr     -> term
// term     -> factor (("-" | "+") factor)*;
// factor   -> unary (("*" | "/") unary)*;
// unary    -> ("+" | "-") unary | primary;
// primary  -> num | "(" expr ")";

bool Parser::atEnd() {
    return peek().type == TokenType::Eof;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::advance() {
    if (!atEnd()) {
        current++;
    }
    return previous();
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    return !atEnd() && peek().type == type;
}

bool Parser::match(TokenType type) {
    if (current < tokens.size() && check(type)) {
        advance();
        return true;
    }
    return false;
}

std::unique_ptr<Expr> Parser::parse() {
    return term();
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> left = factor();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> left = unary();
    while (match(TokenType::Star) || match(TokenType::Slash)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = previous();
        std::unique_ptr<Expr> expr = unary();
        return std::make_unique<Unary>(op, std::move(expr));
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::Number)) {
        return std::make_unique<Literal>(previous());
    }
    if (match(TokenType::LeftParen)) {
        std::unique_ptr<Expr> expr = parse();
        match(TokenType::RightParen);
        return expr;
    }
    throw std::runtime_error("Unexpected token: " + peek().lexeme);
}
