#pragma once

#include <memory>
#include "token.h"

class Visitor {
    public:
        virtual float visitLiteralExpr(class Literal*) = 0;
        virtual float visitUnaryExpr(class Unary*) = 0;
        virtual float visitBinaryExpr(class Binary*) = 0;
};

class Expr {
    public:
        virtual float accept(Visitor*) = 0;
};

class Literal : public Expr {
    public:
        Literal(Token token)
        : token{token} {}

        float accept(Visitor* visitor) override {
            return visitor->visitLiteralExpr(this);
        }
        Token& getToken() { return token; }
    private:
        Token token;
};

class Unary : public Expr {
    public:
        Unary(Token op, std::unique_ptr<Expr>&& right)
        : op{op}, right{std::move(right)} {}

        float accept(Visitor* visitor) override {
            return visitor->visitUnaryExpr(this);
        }
        Token& getOp() { return op; }
        Expr& getRight() { return *right; }
    private:
        Token op;
        std::unique_ptr<Expr> right;
};

class Binary : public Expr {
    public:
        Binary(std::unique_ptr<Expr>&& left, Token op, std::unique_ptr<Expr>&& right)
        : left{std::move(left)}, op{op}, right{std::move(right)} {}

        float accept(Visitor* visitor) override {
            return visitor->visitBinaryExpr(this);
        }
        Expr& getLeft() { return *left; }
         Token& getOp() { return op; }
        Expr& getRight() { return *right; }
    private:
        std::unique_ptr<Expr> left;
         Token op;
        std::unique_ptr<Expr> right;
};

