#include "Debug.hpp"
#include "Chunk.hpp"
#include "Common.hpp"
#include "Value.hpp"
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <ostream>

using namespace clox;

void Disassembler::disassemblerChunk(const Chunk &chunk, const char *name) {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk.count();) {
    offset = disassemblerInstr(chunk, offset);
  }
}

int Disassembler::disassemblerInstr(const Chunk &chunk, int offset) {
  printf("%04d ", offset);

  if (offset > 0 && chunk.getLines()[offset] == chunk.getLines()[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", chunk.getLines()[offset]);
  }

  uint8_t op = chunk.getInstr()[offset];
  switch (OpCode(op)) {
  case OpCode::RETURN:
    return simpleInstr("OP_RETURN", offset);
  case OpCode::CONSTANT:
    return constantInstr("OP_CONSTANT", chunk, offset);
  case OpCode::NIL:
    return simpleInstr("OP_NIL", offset);
  case OpCode::TRUE:
    return simpleInstr("OP_TRUE", offset);
  case OpCode::FALSE:
    return simpleInstr("OP_FALSE", offset);
  case OpCode::EQUAL:
    return simpleInstr("OP_EQUAL", offset);
  case OpCode::GREATER:
    return simpleInstr("OP_GREATER", offset);
  case OpCode::LESS:
    return simpleInstr("OP_LESS", offset);
  case OpCode::ADD:
    return simpleInstr("OP_ADD", offset);
  case OpCode::SUBTRACT:
    return simpleInstr("OP_SUBTRACT", offset);
  case OpCode::MULTIPLY:
    return simpleInstr("OP_MULTIPLY", offset);
  case OpCode::DIVIDE:
    return simpleInstr("OP_DIVIDE", offset);
  case OpCode::NOT:
    return simpleInstr("OP_NOT", offset);
  case OpCode::NEGATE:
    return simpleInstr("OP_NEGATE", offset);
  default:
    return offset + 1;
  }
}

int Disassembler::simpleInstr(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int Disassembler::constantInstr(const char *name, const Chunk &chunk,
                                int offset) {
  uint8_t constant = chunk.getInstr()[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk.getConstants()[constant]);
  printf("'\n");
  return offset + 2;
}
