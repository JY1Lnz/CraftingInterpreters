#include "VirtualMachine.hpp"
#include "Chunk.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Value.hpp"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <iostream>

using namespace clox;
using VM = VirtualMachine;

VM::VirtualMachine() : stackTop{stack} {}

void VM::init() {}

void VM::reset() { stackTop = stack; }

void VM::push(Value value) {
  *stackTop = value;
  ++stackTop;
}

Value VM::pop() {
  assert(stackTop != stack);
  --stackTop;
  return *stackTop;
}

Value VM::top() {
  assert(stackTop != stack);
  return *(stackTop - 1);
}

InterpretResult VM::interpret(Chunk *chunk) {
  this->chunk = chunk;
  ip = chunk->getInstr().cbegin();
  return run();
}

InterpretResult VM::run() {
#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (chunk->getConstants()[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double a = pop();                                                          \
    double b = pop();                                                          \
    push(a op b);                                                              \
  } while (false)

  while (true) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value *slot = stack; slot < stackTop; ++slot) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    Disassembler::disassemblerInstr(
        *chunk, static_cast<int>((ip - chunk->getInstr().cbegin())));
#endif
    uint8_t instr = READ_BYTE();
    switch (static_cast<OpCode>(instr)) {
    case OpCode::RETURN:
      printValue(pop());
      printf("\n");
      return InterpretResult::INTERPRET_OK;
    case OpCode::ADD:
      BINARY_OP(+);
      break;
    case OpCode::SUBTRACT:
      BINARY_OP(-);
      break;
    case OpCode::MULTIPLY:
      BINARY_OP(*);
      break;
    case OpCode::DIVIDE:
      BINARY_OP(/);
      break;
    case OpCode::NEGATE:
      push(-pop());
      break;
    case OpCode::CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    default:
      return InterpretResult::INTERPRET_COMPILE_ERROR;
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}