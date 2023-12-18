#pragma once

#include <string>
#include <string_view>

namespace jlox {

class Scanner;

class Token {
private:
  friend class Scanner;

  enum class Type {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    // Other
    END,
  };

  Type type;
  std::string_view lexeme;
  union {
    std::string_view string;
    double number{0};
  };
  int line;

public:
  Token(Type type, std::string_view lexeme, int line)
      : type{type}, lexeme{std::move(lexeme)}, line{line} {}
  Token(Type type, std::string_view lexeme, std::string_view string, int line)
      : type{type}, lexeme{std::move(lexeme)}, string{std::move(string)},
        line{line} {}
  Token(Type type, std::string_view lexeme, double number, int line)
      : type{type}, lexeme{std::move(lexeme)}, number{number}, line{line} {}

  // const std::string toString() const {
  // }
};
} // namespace jlox