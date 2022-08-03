#pragma once

#include "expr.h"
#include <iostream>

class ASTPrinter : public Visitor {
    public:
        float eval(const std::unique_ptr<Expr>& expr) {
            return expr->accept(this);
        }

    private:
        float visitLiteralExpr(Literal* literal) override {
            return std::stof(literal->getToken().lexeme);
        }

        float visitUnaryExpr(Unary* unary) override {
            if (unary->getOperator().type == TokenType::Minus)
                return -unary->getExpr().accept(this);
            return unary->getExpr().accept(this);
        }

        float visitBinaryExpr(Binary* binary) override {
            auto left = binary->getLeft().accept(this);
            auto right = binary->getRight().accept(this);
            switch (binary->getOperator().type) {
                case TokenType::Plus:
                    return left + right;
                case TokenType::Minus:
                    return left - right;
                case TokenType::Star:
                    return left * right;
                case TokenType::Slash:
                    return left / right;
                default:
                    return 0;
            }
            return 0;
        }
};
