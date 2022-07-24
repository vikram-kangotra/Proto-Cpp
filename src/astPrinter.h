#pragma once

#include "expr.h"
#include <iostream>

class ASTPrinter : public Visitor {
    public:
        ASTPrinter(const std::unique_ptr<Expr>& expr) {
            expr->accept(this);
        }

    private:
        void visitLiteralExpr(Literal* literal) override {
            std::cout << literal->getToken().lexeme;
        }

        void visitUnaryExpr(Unary* unary) override {
            std::cout << unary->getOperator().lexeme << "(";
            unary->getExpr().accept(this);
            std::cout << ")";
        }

        void visitBinaryExpr(Binary* binary) override {
            std::cout << "(";
            binary->getLeft().accept(this);
            std::cout << " " << binary->getOperator().lexeme << " ";
            binary->getRight().accept(this);
            std::cout << ")";
        }
};
