#include "VirtualMachine.hpp"
#include "Chunk.hpp"
#include "Common.hpp"
#include "Debug.hpp"
#include "Value.hpp"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdarg.h>

using namespace clox;
using VM = VirtualMachine;

bool VM::isFalsey(Value value) {
  return value.isNil() || (value.isBool() && !value.getBool());
}

bool VM::valueEqual(Value a, Value b) const {
  if (a.getType() != b.getType())
    return false;
  switch (a.getType()) {
  case Value::Type::BOOL:
    return a.getBool() == b.getBool();
  case Value::Type::NIL:
    return true;
  case Value::Type::NUMBER:
    return a.getNumber() == b.getNumber();
  default:
    return false;
  }
}

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

const Value &VM::peek(int dis) { return stackTop[-1 - dis]; }

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
    if (!peek(0).isNumber() || !peek(1).isNumber()) {                          \
      runtimeError("Operands must be number.");                                \
      return InterpretResult::INTERPRET_RUNTIME_ERROR;                         \
    }                                                                          \
    double b = pop().getNumber();                                              \
    double a = pop().getNumber();                                              \
    push(Value{a op b});                                                       \
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
    case OpCode::EQUAL: {
      Value b = pop();
      Value a = pop();
      push(Value{valueEqual(a, b)});
      break;
    }
    case OpCode::GREATER:
      BINARY_OP(>);
      break;
    case OpCode::LESS:
      BINARY_OP(<);
      break;
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
    case OpCode::NOT:
      push(Value{isFalsey(pop())});
      break;
    case OpCode::NEGATE:
      if (!peek(0).isNumber()) {
        return InterpretResult::INTERPRET_RUNTIME_ERROR;
      }
      push(Value{-pop().getNumber()});
      break;
    case OpCode::CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OpCode::NIL:
      push(Value{});
      break;
    case OpCode::TRUE:
      push(Value{true});
      break;
    case OpCode::FALSE:
      push(Value{false});
      break;
    default:
      return InterpretResult::INTERPRET_COMPILE_ERROR;
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

void VM::runtimeError(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = ip - chunk->getInstr().begin() - 1;
  int line = chunk->getLines()[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  reset();
}