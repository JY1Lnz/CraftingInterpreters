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

protected:
  bool isFalsey(Value value);
  bool valueEqual(Value a, Value b) const;

public:
  explicit VirtualMachine();

  void init();
  void reset();

  /// stack
  void push(Value value);
  Value pop();
  Value top();

  const Value &peek(int dis);

  InterpretResult interpret(Chunk *chunk);
  InterpretResult run();

  void runtimeError(const char *format, ...);
};
} // namespace clox