#pragma once

#include "Chunk.hpp"
#include "Common.hpp"
#include "Token.hpp"
#include <cstdint>
namespace clox {
enum class Precedence;

class Parser;
using ParseFn = void (Parser::*)();

void errorAt(const Token &token, const char *msg);

struct ParseRule {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
};

class Parser {
private:
  Token current;
  Token previous;
  Scanner *sc{nullptr};
  Chunk *ch{nullptr};

  mutable bool hadError{false};
  mutable bool panicMode{false};

  using TT = Token::Type;

public:
  Parser() = default;

  void init(Scanner *sc, Chunk *ch);

  const Token &getPrevious() const { return previous; }
  const Token &getCurrent() const { return current; }

  void setError(bool error) const { hadError = error; }
  bool isError() const { return hadError; }
  void setPanic(bool panic) const { panicMode = panic; }
  bool isPanic() const { return panicMode; }

  void advance();
  void consume(Token::Type type, const char *msg);
  void endCompiler();

  void grouping();
  void expression();

  uint8_t makeConstant(double value);

  void emitByte(uint8_t byte);
  void emitByte(OpCode op);
  void emitBytes(uint8_t byte1, uint8_t byte2);
  void emitBytes(OpCode byte1, uint8_t byte2);
  void emitBytes(uint8_t byte1, OpCode byte2);
  void emitBytes(OpCode byte1, OpCode byte2);
  void emitReturn();
  void emitConstant(double value);

  void parsePrecedence(Precedence prec);
  const ParseRule &getRule(TT type) const;

  void number();
  void unary();
  void binary();
};

bool compile(const char *source, Chunk &chunk);
void advance();
} // namespace clox