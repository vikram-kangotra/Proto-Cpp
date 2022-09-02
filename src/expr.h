#pragma once

#include <memory>
#include <any>
#include "token.h"

class ExprVisitor {
    public:
        virtual std::any visitLiteralExpr(class LiteralExpr*) = 0;
        virtual std::any visitUnaryExpr(class UnaryExpr*) = 0;
        virtual std::any visitBinaryExpr(class BinaryExpr*) = 0;
};

class Expr {
    public:
        virtual std::any accept(ExprVisitor*) = 0;
};

class LiteralExpr : public Expr {
    public:
        LiteralExpr(std::any value)
        : value{value} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitLiteralExpr(this);
        }
        std::any& getValue() { return value; }
    private:
        std::any value;
};

class UnaryExpr : public Expr {
    public:
        UnaryExpr(Token op, std::unique_ptr<Expr>&& right)
        : op{op}, right{std::move(right)} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitUnaryExpr(this);
        }
        Token& getOp() { return op; }
        std::unique_ptr<Expr>& getRight() { return right; }
    private:
        Token op;
        std::unique_ptr<Expr> right;
};

class BinaryExpr : public Expr {
    public:
        BinaryExpr(std::unique_ptr<Expr>&& left, Token op, std::unique_ptr<Expr>&& right)
        : left{std::move(left)}, op{op}, right{std::move(right)} {}

        std::any accept(ExprVisitor* visitor) override {
            return visitor->visitBinaryExpr(this);
        }
        std::unique_ptr<Expr>& getLeft() { return left; }
         Token& getOp() { return op; }
        std::unique_ptr<Expr>& getRight() { return right; }
    private:
        std::unique_ptr<Expr> left;
         Token op;
        std::unique_ptr<Expr> right;
};

class StmtVisitor {
    public:
        virtual std::any visitExpressionStmt(class ExpressionStmt*) = 0;
        virtual std::any visitPrintStmt(class PrintStmt*) = 0;
};

class Stmt {
    public:
        virtual std::any accept(StmtVisitor*) = 0;
};

class ExpressionStmt : public Stmt {
    public:
        ExpressionStmt(std::unique_ptr<Expr>&& expression)
        : expression{std::move(expression)} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitExpressionStmt(this);
        }
        std::unique_ptr<Expr>& getExpression() { return expression; }
    private:
        std::unique_ptr<Expr> expression;
};

class PrintStmt : public Stmt {
    public:
        PrintStmt(std::unique_ptr<Expr>&& expression)
        : expression{std::move(expression)} {}

        std::any accept(StmtVisitor* visitor) override {
            return visitor->visitPrintStmt(this);
        }
        std::unique_ptr<Expr>& getExpression() { return expression; }
    private:
        std::unique_ptr<Expr> expression;
};

