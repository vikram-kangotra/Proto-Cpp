#pragma once

#include <string>

enum TokenType {
    LeftParen, RightParen, LeftBrace, RightBrace, 
    Comma, Dot, Minus, Plus, Semicolon, Slash, Star,

    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,

    Identifier, String, Number,

    And, Class, Else, False, Fun, For, If, Nil, Or,
    Print, Return, Super, This, True, Var, While,

    Eof, Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType type, std::string lexeme) : type(type), lexeme(lexeme) {}
};
