#pragma once

#include <cstdint>

#define DEBUG_TRACE_EXECUTION
#define DEBUG_PRINT_CODE

namespace clox {

enum class OpCode : uint8_t {
  CONSTANT,
  NIL,
  TRUE,
  FALSE,
  EQUAL,
  GREATER,
  LESS,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  NOT,
  NEGATE,
  RETURN,
};

}
