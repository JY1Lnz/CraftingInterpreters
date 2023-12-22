#include "Chunk.hpp"
#include "Common.hpp"
#include "Value.hpp"
#include <cstdint>

using namespace clox;

void Chunk::addOperator(OpCode op, int line) {
  instr.push_back(static_cast<uint8_t>(op));
  lines.push_back(line);
}

void Chunk::addValue(uint8_t val, int line) {
  instr.push_back(val);
  lines.push_back(line);
}

int Chunk::addConstant(Value value) {
  constants.push_back(std::move(value));
  return constants.size() - 1;
}