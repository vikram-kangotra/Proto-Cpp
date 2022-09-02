#include "parser.h"
#include <exception>
#include <iostream>

// program      -> statement* EOF;
// statement    -> exprStmt | printStmt;
// exprStmt     -> expr ";";
// printStmt    -> "print" expr ";";
// expr         -> equality
// equality     -> comparison (("!=" | "==") comparison)*;
// comparison   -> term ((">" | ">=" | "<" | "<=") term)*;
// term         -> factor (("-" | "+") factor)*;
// factor       -> UnaryExpr (("*" | "/") UnaryExpr)*;
// UnaryExpr        -> ("+" | "-") UnaryExpr | primary;
// primary      -> num | "(" expr ")";

bool Parser::atEnd() {
    return current >= tokens.size() || peek().type == TokenType::Eof;
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

void Parser::consume(TokenType type, const std::string& err) {
    if (!match(type))
        throw std::runtime_error(err);
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

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!atEnd()) {
        statements.push_back(statement());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match(TokenType::Print)) return printStatement();
    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement() {
    std::unique_ptr<Expr> value = expr();
    consume(TokenType::Semicolon, "Expect ';' after value.'");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::expressionStatement() {
    std::unique_ptr<Expr> expression = expr();
    consume(TokenType::Semicolon, "Expect ';' afer expression");
    return std::make_unique<ExpressionStmt>(std::move(expression));
}

std::unique_ptr<Expr> Parser::expr() {
    return equality();
}

std::unique_ptr<Expr> Parser::equality() {
    auto left = comparison();
    while (match(TokenType::BangEqual) || match(TokenType::EqualEqual)) {
        auto op = previous();
        auto right = comparison();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto left = term();
    while (match(TokenType::Greater) || match(TokenType::GreaterEqual) || match(TokenType::Less) || match(TokenType::LessEqual)) {
        auto op = previous();
        auto right = term();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::term() {
    std::unique_ptr<Expr> left = factor();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = previous();
        std::unique_ptr<Expr> right = factor();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::factor() {
    std::unique_ptr<Expr> left = unary();
    while (match(TokenType::Star) || match(TokenType::Slash)) {
        Token op = previous();
        std::unique_ptr<Expr> right = unary();
        left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
    }
    return left;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = previous();
        std::unique_ptr<Expr> expr = unary();
        return std::make_unique<UnaryExpr>(op, std::move(expr));
    }
    if (match(TokenType::LeftParen)) {
        std::unique_ptr<Expr> exp = expr();
        consume(TokenType::RightParen, "Expected ')'");
        return exp;
    }
    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(TokenType::True)) return std::make_unique<LiteralExpr>(false);
    if (match(TokenType::True)) return std::make_unique<LiteralExpr>(true);

    if (match(TokenType::Number) || match(TokenType::String)) {
        return std::make_unique<LiteralExpr>(previous().literal);
    }
    if (match(TokenType::LeftParen)) {
        std::unique_ptr<Expr> exp = expr();
        match(TokenType::RightParen);
        return exp;
    }
    throw std::runtime_error("Parsing Error - Unexpected token: " + peek().lexeme);
}
