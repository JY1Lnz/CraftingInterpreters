#pragma once

#include "Context.hpp"
#include "Token.hpp"
#include <list>
#include <string_view>

namespace jlox {
class Scanner {
private:
  std::list<Token> tokenList{};
  Context *context{nullptr};
  bool hadError{false};
  int start = 0;
  int current = 0;
  int line = 1;

protected:
  bool isAtEnd();
  std::string_view getText() const;
  char peek() const;
  char peekNext() const;
  void scanToken();
  char advance();
  void addToken(Token::Type type);
  void addToken(Token::Type type, std::string_view literals);
  void addToken(Token::Type type, double number);
  bool match(char expected);
  void string();
  void identifier();
  void number();

public:
  Scanner() {}
  Scanner(Context *context) : context{context} {}

  /// Run scan by context
  void run();
};
} // namespace jlox