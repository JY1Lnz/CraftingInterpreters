#include "Scanner.hpp"
#include "Token.hpp"
#include <cctype>
#include <string_view>
#include <variant>

using namespace jlox;

bool Scanner::isAtEnd() { return current >= getText().length(); }

std::string_view Scanner::getText() const { return context->getText(); }

char Scanner::peek() const { return getText().at(current); }

char Scanner::peekNext() const {
  if (current + 1 >= getText().length())
    return '\0';
  return getText().at(current + 1);
}

void Scanner::scanToken() {
  using TT = Token::Type;
  char c = advance();
  switch (c) {
  case '(':
    addToken(TT::LEFT_PAREN);
    break;
  case '=':
    addToken(match('=') ? TT::EQUAL_EQUAL : TT::EQUAL);
    break;
  case '!':
    addToken(match('=') ? TT::BANG_EQUAL : TT::BANG);
    break;
  case ' ':
  case '\r':
  case '\t':
    break;
  case '\n':
    line++;
    break;
  case '"':
    string();
    break;
  default:
    if (std::isalpha(c)) {
      identifier();
    } else if (std::isdigit(c)) {
      number();
    } else {
      hadError = true;
    }
    break;
  }
}

char Scanner::advance() { return getText().at(current++); }

void Scanner::addToken(Token::Type type) {
  tokenList.emplace_back(type, getText().substr(start, current - start), line);
}

void Scanner::addToken(Token::Type type, std::string_view string) {
  tokenList.emplace_back(type, getText().substr(start, current - start),
                         std::move(string), line);
}

void Scanner::addToken(Token::Type type, double number) {
  tokenList.emplace_back(type, getText().substr(start, current - start), number,
                         line);
}

bool Scanner::match(char expected) {
  if (isAtEnd())
    return false;
  if (peek() != expected)
    return false;
  current++;
  return true;
}

void Scanner::string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') {
      line++;
    }
    advance();
  }

  // TODO: Error
  if (isAtEnd()) {
    hadError = true;
    return;
  }

  // eat closing ".
  advance();
  addToken(Token::Type::STRING,
           getText().substr(start + 1, current - start - 2));
}

void Scanner::identifier() {
  while (std::isalpha(peek()) || std::isdigit(peek())) {
    advance();
  }
  addToken(Token::Type::IDENTIFIER);
}

void Scanner::number() {
  while (!isAtEnd() && std::isdigit(peek())) {
    advance();
  }
  // Look for a fractional part.
  if (!isAtEnd() && peek() == '.' && std::isdigit(peekNext())) {
    // eat .
    advance();

    while (std::isdigit(peek()))
      advance();
  }

  // TODO: use more safety method
  addToken(Token::Type::NUMBER,
           std::stod(getText().substr(start, current - start).data()));
}

void Scanner::run() {
  while (!isAtEnd()) {
    start = current;
    scanToken();
  }
  tokenList.emplace_back(Token::Type::END, "", line);
}