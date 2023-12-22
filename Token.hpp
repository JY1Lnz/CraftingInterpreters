#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace clox {

class Scanner;
class Parser;
class Chunk;

class Token {
private:
  friend class Scanner;
  friend class Parser;
  friend void errorAt(const Token &token, const char *msg);
  friend bool compile(const char *source, Chunk &chunk);

public:
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
    ERROR,
    END,
    COUNT,
    NONE,
  };

private:
  Type type;
  const char *start;
  int length;
  int line;

  std::string_view lexeme;
  union {
    std::string_view string;
    double number{0};
  };

public:
  static std::unordered_map<std::string_view, Type> reservedWord;
  static Type findType(std::string_view iden);

public:
  static Token makeToken(Type type, const char *start, int length, int line);

public:
  Token();
  Token(Type type, const char *start, int length, int line);

  Token(Type type, std::string_view lexeme, int line)
      : type{type}, lexeme{std::move(lexeme)}, line{line} {}
  Token(Type type, std::string_view lexeme, std::string_view string, int line)
      : type{type}, lexeme{std::move(lexeme)}, string{std::move(string)},
        line{line} {}
  Token(Type type, std::string_view lexeme, double number, int line)
      : type{type}, lexeme{std::move(lexeme)}, number{number}, line{line} {}
  int getLine() const { return line; }
  Type getType() const { return type; }
  const char *getStart() const { return start; }

  // const std::string toString() const {
  // }
};
} // namespace clox