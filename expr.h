#pragma once

#include <memory>
#include "token.h"

class Visitor {
    public:
        virtual void visitLiteralExpr(class Literal* literal) = 0;
        virtual void visitUnaryExpr(class Unary* unary) = 0;
        virtual void visitBinaryExpr(class Binary* binary) = 0;
};

class Expr {
    public:
        virtual void accept(Visitor* visitor) = 0;
};

class Literal : public Expr {
    public:
        Literal(Token token) : token{token} {}
        void accept(Visitor* visitor) override {
            visitor->visitLiteralExpr(this);
        }

        Token& getToken() { return token; }
    private:
        Token token;
};

class Unary : public Expr {
    public:
        Unary(Token op, std::unique_ptr<Expr>&& expr)
        : op{op}, expr{std::move(expr)} {}

        void accept(Visitor* visitor) override {
            visitor->visitUnaryExpr(this);
        }

        Token& getOperator() { return op; }
        Expr& getExpr() { return *expr; }
    private:
        Token op;
        std::unique_ptr<Expr> expr;
};

class Binary : public Expr {
    public:
        Binary(std::unique_ptr<Expr>&& left, Token op, std::unique_ptr<Expr>&& right)
        : left{std::move(left)}, op{op}, right{std::move(right)} {}

        void accept(Visitor* visitor) override {
            visitor->visitBinaryExpr(this);
        }

        Expr& getLeft() { return *left; }
        Token& getOperator() { return op; }
        Expr& getRight() { return *right; }
    private:
        std::unique_ptr<Expr> left;
        Token op;
        std::unique_ptr<Expr> right;
};
