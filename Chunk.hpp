#pragma once

#include "Common.hpp"
#include "Value.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace clox {
class Chunk {
  std::vector<uint8_t> instr{};
  std::vector<Value> constants{};
  // bytecode's line information
  std::vector<int> lines{};

public:
  const std::vector<uint8_t> &getInstr() const { return instr; }
  const std::vector<Value> &getConstants() const { return constants; }
  const std::vector<int> &getLines() const { return lines; }
  void addOperator(OpCode op, int line);
  void addValue(uint8_t val, int line);
  int addConstant(Value value);
  std::size_t count() const { return instr.size(); }
};
} // namespace clox