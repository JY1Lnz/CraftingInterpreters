#include "Scanner.hpp"
#include "Token.hpp"
#include "VirtualMachine.hpp"
#include <cctype>
#include <cinttypes>
#include <cstring>
#include <string_view>
#include <unordered_map>

using namespace clox;

static bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::isAtEnd() const { return *current == '\0'; }

char Scanner::advance() { return *current++; }

char Scanner::peek() const { return *current; }

char Scanner::peekNext() const {
  if (isAtEnd())
    return 0;
  return *(current + 1);
}

Token Scanner::makeToken(Token::Type type) {
  return Token::makeToken(type, start, static_cast<int>(current - start), line);
}

Token Scanner::makeErrorToken(Token::Type type, const char *msg) {
  return Token::makeToken(type, msg, strlen(msg), line);
}

bool Scanner::match(char expected) {
  if (isAtEnd())
    return false;
  if (*current != expected)
    return false;
  current++;
  return true;
}

void Scanner::skipWhiteSpace() {
  while (true) {
    char c = peek();
    switch (c) {
    case ' ':
    case '\t':
    case '\r':
      advance();
      break;
    case '\n':
      line++;
      advance();
      break;
    case '/':
      if (peekNext() == '/') {
        while (peek() != '\n' && !isAtEnd())
          advance();
      } else {
        return;
      }
      break;
    default:
      return;
    }
  }
}

Token Scanner::toString() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n')
      line++;
    advance();
  }

  if (isAtEnd())
    return makeErrorToken(Token::Type::ERROR, "Unterminated string.");
  // eat `"`
  advance();
  return makeToken(Token::Type::STRING);
}

Token Scanner::toNumber() {
  while (isdigit(peek()))
    advance();
  if (peek() == '.' && isdigit(peekNext())) {
    advance();

    while (isdigit(peek()))
      advance();
  }
  return makeToken(Token::Type::NUMBER);
}

Token Scanner::toIdentifier() {
  while (isAlpha(peek()) || isDigit(peek()))
    advance();
  return makeToken(identifierType());
}

Token::Type Scanner::identifierType() {
  return Token::findType(
      std::string_view{start, static_cast<size_t>(current - start)});
}

void Scanner::init(const char *source) {
  start = source;
  current = source;
  line = 1;
}

Token Scanner::scanToken() {
  using TT = Token::Type;
  skipWhiteSpace();
  start = current;
  if (isAtEnd())
    return makeToken(TT::END);
  char c = advance();

  if (isdigit(c))
    return toNumber();

  if (isAlpha(c))
    return toIdentifier();

  switch (c) {
  case '(':
    return makeToken(TT::LEFT_PAREN);
  case ')':
    return makeToken(TT::RIGHT_PAREN);
  case '{':
    return makeToken(TT::LEFT_BRACE);
  case '}':
    return makeToken(TT::RIGHT_BRACE);
  case ';':
    return makeToken(TT::SEMICOLON);
  case ',':
    return makeToken(TT::COMMA);
  case '.':
    return makeToken(TT::DOT);
  case '-':
    return makeToken(TT::MINUS);
  case '+':
    return makeToken(TT::PLUS);
  case '/':
    return makeToken(TT::SLASH);
  case '*':
    return makeToken(TT::STAR);
  case '!':
    return makeToken(match('=') ? TT::BANG_EQUAL : TT::BANG);
  case '=':
    return makeToken(match('=') ? TT::EQUAL_EQUAL : TT::EQUAL);
  case '<':
    return makeToken(match('=') ? TT::LESS_EQUAL : TT::LESS);
  case '>':
    return makeToken(match('=') ? TT::GREATER_EQUAL : TT::GREATER);
  case '"':
    return toString();
  default:
    break;
  }

  return Token::makeToken(Token::Type::ERROR, "Unexpected character.", 21,
                          line);
}