#include "Compiler.hpp"
#include "Chunk.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Scanner.hpp"
#include "Token.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

using namespace clox;

namespace clox {
enum class Precedence {
  NONE,
  ASSIGNMENT, // =
  OR,         // or
  AND,        // and
  EQUALITY,   // == !=
  COMPARISON, // < > <= >=
  TERM,       // + -
  FACTOR,     // * /
  UNARY,      // ! -
  CALL,       // . ()
  PRIMARY
};

} // namespace clox

static std::array<ParseRule, static_cast<uint>(Token::Type::COUNT)> relus{{
    {&Parser::grouping, nullptr, Precedence::NONE},      // LEFT_PAREN
    {nullptr, nullptr, Precedence::NONE},                // RIGHT_PAREN
    {nullptr, nullptr, Precedence::NONE},                // LEFT_BRACE
    {nullptr, nullptr, Precedence::NONE},                // RIGHT_BRACE
    {nullptr, nullptr, Precedence::NONE},                // COMMA
    {nullptr, nullptr, Precedence::NONE},                // DOT
    {&Parser::unary, &Parser::binary, Precedence::TERM}, // MINUS
    {nullptr, &Parser::binary, Precedence::TERM},        // PLUS
    {nullptr, nullptr, Precedence::NONE},                // SEMICOLON
    {nullptr, &Parser::binary, Precedence::FACTOR},      // SLASH
    {nullptr, &Parser::binary, Precedence::FACTOR},      // STAR
    {nullptr, nullptr, Precedence::NONE},                // BANG
    {nullptr, nullptr, Precedence::NONE},                // BANG_EQUAL
    {nullptr, nullptr, Precedence::NONE},                // EQUAL
    {nullptr, nullptr, Precedence::NONE},                // EQUAL_EQUAL
    {nullptr, nullptr, Precedence::NONE},                // GREATER
    {nullptr, nullptr, Precedence::NONE},                // GREATER_EQUAL
    {nullptr, nullptr, Precedence::NONE},                // LESS
    {nullptr, nullptr, Precedence::NONE},                // LESS_EQUAL
    {nullptr, nullptr, Precedence::NONE},                // IDENTIFIER
    {nullptr, nullptr, Precedence::NONE},                // STRING
    {&Parser::number, nullptr, Precedence::NONE},        // NUMBER
    {nullptr, nullptr, Precedence::NONE},                // AND
    {nullptr, nullptr, Precedence::NONE},                // CLASS
    {nullptr, nullptr, Precedence::NONE},                // ELSE
    {nullptr, nullptr, Precedence::NONE},                // FALSE
    {nullptr, nullptr, Precedence::NONE},                // FOR
    {nullptr, nullptr, Precedence::NONE},                // FUN
    {nullptr, nullptr, Precedence::NONE},                // IF
    {nullptr, nullptr, Precedence::NONE},                // NIL
    {nullptr, nullptr, Precedence::NONE},                // OR
    {nullptr, nullptr, Precedence::NONE},                // PRINT
    {nullptr, nullptr, Precedence::NONE},                // RETURN
    {nullptr, nullptr, Precedence::NONE},                // SUPER
    {nullptr, nullptr, Precedence::NONE},                // THIS
    {nullptr, nullptr, Precedence::NONE},                // TRUE
    {nullptr, nullptr, Precedence::NONE},                // VAR
    {nullptr, nullptr, Precedence::NONE},                // WHILE
    {nullptr, nullptr, Precedence::NONE},                // ERROR
    {nullptr, nullptr, Precedence::NONE},                // END
}};

void clox::errorAt(const Token &token, const char *msg) {
  fprintf(stderr, "[line %d] Error", token.getLine());
  if (token.getType() == Token::Type::END) {
    fprintf(stderr, " at end");
  } else if (token.getType() == Token::Type::ERROR) {
  } else {
    fprintf(stderr, " at '%.*s'", token.length, token.start);
  }

  fprintf(stderr, ": %s\n", msg);
}

static void errorAtCurrent(const Parser &parser, const char *msg) {
  if (parser.isPanic())
    return;
  parser.setPanic(true);
  errorAt(parser.getPrevious(), msg);
  parser.setError(true);
}

static void error(const Parser &parser, const char *msg) {
  if (parser.isPanic())
    return;
  parser.setPanic(true);
  errorAt(parser.getCurrent(), msg);
  parser.setError(true);
}

void Parser::init(Scanner *sc, Chunk *ch) {
  this->sc = sc;
  this->ch = ch;
}

void Parser::advance() {
  previous = current;
  while (true) {
    current = sc->scanToken();
    if (current.getType() != Token::Type::ERROR)
      break;
    errorAtCurrent(*this, getCurrent().start);
  }
}

void Parser::consume(Token::Type type, const char *msg) {
  if (current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(*this, msg);
}

void Parser::endCompiler() {
  emitReturn();
#ifdef DEBUG_PRINT_CODE
  if (!hadError) {
    Disassembler::disassemblerChunk(*ch, "code");
  }
#endif
}

void Parser::grouping() {
  expression();
  consume(TT::RIGHT_PAREN, "Expect ')' after expression.");
}

void Parser::expression() {
  // all file is a assignment token
  parsePrecedence(Precedence::ASSIGNMENT);
}

uint8_t Parser::makeConstant(double value) {
  int constant = ch->addConstant(value);
  if (constant > UINT8_MAX) {
    error(*this, "Too many constants in one chunk.");
    return 0;
  }
  return static_cast<uint8_t>(constant);
}

void Parser::emitByte(uint8_t byte) {
  ch->addOperator(static_cast<OpCode>(byte), previous.getLine());
}

void Parser::emitByte(OpCode op) { ch->addOperator(op, previous.getLine()); }

void Parser::emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

void Parser::emitBytes(OpCode byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

void Parser::emitBytes(uint8_t byte1, OpCode byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

void Parser::emitBytes(OpCode byte1, OpCode byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

void Parser::emitReturn() { emitByte(OpCode::RETURN); }

void Parser::emitConstant(double value) {
  emitBytes(OpCode::CONSTANT, makeConstant(value));
}

void Parser::parsePrecedence(Precedence prec) {
  // get the next token, put current token in previous.
  advance();
  // The first operand is prefix operand, so do the prefix op
  ParseFn prefixRule = getRule(previous.getType()).prefix;
  if (prefixRule == nullptr) {
    error(*this, "Expect expression.");
    return;
  }
  (this->*prefixRule)();
  // try to check is or not a binary op
  while (prec <= getRule(current.type).precedence) {
    advance();
    ParseFn infixRule = getRule(previous.type).infix;
    (this->*infixRule)();
  }
}

const ParseRule &Parser::getRule(TT type) const {
  return relus[static_cast<uint>(type)];
}

void Parser::number() {
  double value = strtod(previous.getStart(), nullptr);
  emitConstant(value);
}

void Parser::unary() {
  TT op = previous.getType();
  parsePrecedence(Precedence::UNARY);
  switch (op) {
  case TT::MINUS:
    emitByte(OpCode::NEGATE);
    break;
  default:
    return;
  }
}

void Parser::binary() {
  Token::Type operatorType = previous.getType();
  const ParseRule &rule = getRule(operatorType);
  parsePrecedence(
      static_cast<Precedence>(static_cast<uint>(rule.precedence) + 1));
  switch (operatorType) {
  case TT::PLUS:
    emitByte(OpCode::ADD);
    break;
  case TT::MINUS:
    emitByte(OpCode::SUBTRACT);
    break;
  case TT::STAR:
    emitByte(OpCode::MULTIPLY);
    break;
  case TT::SLASH:
    emitByte(OpCode::DIVIDE);
    break;
  default:
    return;
  }
}

bool clox::compile(const char *source, Chunk &chunk) {
  Scanner sc;
  sc.init(source);
  Parser parser;
  parser.init(&sc, &chunk);
  parser.advance();
  parser.expression();
  parser.consume(Token::Type::END, "Expect end of expression.");
  parser.endCompiler();
  return !parser.isError();
}

void clox::advance() {}