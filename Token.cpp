#include "Token.hpp"
#include <string_view>
#include <unordered_map>

using namespace clox;

std::unordered_map<std::string_view, Token::Type> Token::reservedWord{
    {"and", Token::Type::AND},     {"class", Token::Type::CLASS},
    {"else", Token::Type::ELSE},   {"if", Token::Type::IF},
    {"nil", Token::Type::NIL},     {"or", Token::Type::OR},
    {"print", Token::Type::PRINT}, {"return", Token::Type::RETURN},
    {"super", Token::Type::SUPER}, {"var", Token::Type::VAR},
    {"while", Token::Type::WHILE}, {"false", Token::Type::FALSE},
    {"for", Token::Type::FOR},     {"fun", Token::Type::FUN},
    {"this", Token::Type::THIS},   {"true", Token::Type::TRUE},
};

Token::Type Token::findType(std::string_view iden) {
  const auto it = reservedWord.find(iden);
  if (it == reservedWord.end()) {
    return Type::IDENTIFIER;
  }
  return it->second;
}

Token Token::makeToken(Type type, const char *start, int length, int line) {
  return Token{type, start, length, line};
}

Token::Token() : type{Type::NONE}, start{nullptr}, length{0}, line{0} {}

Token::Token(Type type, const char *start, int length, int line)
    : type{type}, start{start}, length{length}, line{line} {}