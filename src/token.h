#pragma once

#include <string>
#include <any>

enum TokenType {
    LeftParen, RightParen, LeftBrace, RightBrace, 
    Comma, Dot, Minus, Plus, Semicolon, Slash, Star, 

    Bang, BangEqual,
    Equal, EqualEqual,
    Greater, GreaterEqual,
    Less, LessEqual,

    Identifier, String, Number,

    And, Break, Class, Continue, Do, Else, ElseIf, 
    False, Fun, For, If, In, Let, Nil, Or,
    Print, Return, Super, Static, Struct, Switch, 
    This, True, Var, While,

    Eof, Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;

    Token(TokenType type, std::string lexeme, std::any literal = nullptr) : type{type}, lexeme{lexeme}, literal{literal} {}
};
