#pragma once

#include "Value.hpp"
#include <cstdint>
#include <vector>
namespace clox {

#define STACK_MAC 256

enum class InterpretResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
};


class Chunk;

class VirtualMachine {
private:
  Chunk *chunk;
  std::vector<uint8_t>::const_iterator ip;
  Value stack[STACK_MAC];
  Value *stackTop{nullptr};

public:
  VirtualMachine();

  void init();
  void reset();

  /// stack
  void push(Value value);
  Value pop();
  Value top();

  InterpretResult interpret(Chunk *chunk);
  InterpretResult run();
};
} // namespace clox