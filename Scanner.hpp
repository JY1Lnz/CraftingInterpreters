#pragma once

#include "Token.hpp"
namespace clox {
class Scanner {
private:
  const char *start{nullptr};
  const char *current{nullptr};
  int line{0};

protected:
  bool isAtEnd() const;
  char advance();
  char peek() const;
  char peekNext() const;
  Token makeToken(Token::Type type);
  Token makeErrorToken(Token::Type type, const char *msg);
  bool match(char expected);
  void skipWhiteSpace();
  Token toString();
  Token toNumber();
  Token toIdentifier();
  Token::Type identifierType();

public:
  Scanner() = default;
  void init(const char *source);
  Token scanToken();
  const char *getStart() const { return start; }
  const char *getCurr() const { return current; }
  int getLine() const { return line; }
};

} // namespace clox