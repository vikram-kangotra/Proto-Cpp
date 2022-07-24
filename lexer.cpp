#include "lexer.h"

Token Lexer::nextToken() {
    if (peek() == '\0') {
        return Token(TokenType::Eof, "");
    }

    skipWhitespace();

    start = current;

    auto ch = advance();

    switch (ch) {
        case '(': return Token(TokenType::LeftParen, "("); break;
        case ')': return Token(TokenType::RightParen, ")"); break;
        case '{': return Token(TokenType::LeftBrace, "{"); break;
        case '}': return Token(TokenType::RightBrace, "}"); break;
        case ';': return Token(TokenType::Semicolon, ";"); break;
        case ',': return Token(TokenType::Comma, ","); break;
        case '.': return Token(TokenType::Dot, "."); break;
        case '-': return Token(TokenType::Minus, "-"); break;
        case '+': return Token(TokenType::Plus, "+"); break;
        case '*': return Token(TokenType::Star, "*"); break;
        case '=': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::Equal, "==");
            }
            return Token(TokenType::Equal, "=");
        } break;
        case '!': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::BangEqual, "!=");
            }
            return Token(TokenType::Bang, "!");
        } break;
        case '<': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::LessEqual, "<=");
            }
            return Token(TokenType::Less, "<");
        } break;
        case '>': {
            if (peek() == '=') {
                advance();
                return Token(TokenType::GreaterEqual, ">=");
            }
            return Token(TokenType::Greater, ">");
        } break;
        case '/': {
            if (peek() == '/') {
                while (peek() != '\n' && peek() != '\0') {
                    advance();
                }
                return nextToken();
            }
            return Token(TokenType::Slash, "/");
        } break;
        case '"': { return string(); } break;
        default: {
            if (isdigit(ch)) {
                return number();
            } else if (isalpha(ch)) {
                return identifier();
            } else {
                return Token(TokenType::Unknown, std::string(1, ch));
            }
        }
    }
}

std::vector<Token> Lexer::getTokens() {
    std::vector<Token> tokens;
    while (true) {
        auto token = nextToken();
        if (token.type == TokenType::Eof) {
            break;
        }
        tokens.push_back(token);
    }
    return tokens;
}

void Lexer::skipWhitespace() {
    while (true) {
        auto ch = peek();
        switch (ch) {
            case ' ':
            case '\t':
            case '\r': {
                advance();
            } break;
            case '\n': {
                line++;
                advance();
            } break;
            default: {
                return;
            } break;
        }
    }
}

void Lexer::skipComment() {
    while (true) {
        auto ch = peek();
        switch (ch) {
            case '\n': {
                line++;
                advance();
                return;
            } break;
            default: {
                advance();
            } break;
        }
    }
}

char Lexer::peek_next() {
    if (current + 1 >= input.size()) {
        return '\0';
    }
    return input[current + 1];
}

char Lexer::peek() {
    if (current >= input.size()) {
        return '\0';
    }
    return input[current];
}

char Lexer::advance() {
    current++;
    return input[current - 1];
}

Token Lexer::identifier() {
    while (isalpha(peek()) || isdigit(peek())) {
        advance();
    }
    return Token(TokenType::Identifier, input.substr(start, current - start));
}

Token Lexer::number() {
    while (isdigit(peek())) {
        advance();
    }
    
    if (peek() == '.' && isdigit(peek_next())) {
        advance();
        while (isdigit(peek())) {
            advance();
        }
    }

    return Token(TokenType::Number, input.substr(start, current - start));
}

Token Lexer::string() {
    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    if (peek() == '\0') {
        return Token(TokenType::Unknown, "Unterminated string");
    }
    advance();
    return Token(TokenType::String, input.substr(start + 1, current - start - 2));
}
