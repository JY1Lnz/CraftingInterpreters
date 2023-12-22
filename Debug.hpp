#pragma once

#include <cstdint>
#include <vector>

namespace clox {
class Chunk;

class Disassembler {
public:
  static void disassemblerChunk(const Chunk &chunk, const char *name);
  static int disassemblerInstr(const Chunk &chunk, int offset);
  static int simpleInstr(const char *name, int offset);
  static int constantInstr(const char *name, const Chunk &chunk, int offset);
};
} // namespace clox