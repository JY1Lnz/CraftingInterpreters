#include "Chunk.hpp"
#include "Common.hpp"
#include "Compiler.hpp"
#include "Debug.hpp"
#include "VirtualMachine.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

static bool hadError{false};
static const char *File{nullptr};

static clox::InterpretResult interpret(const char *source) {
  clox::Chunk chunk;
  if (!clox::compile(source, chunk)) {
    return clox::InterpretResult::INTERPRET_COMPILE_ERROR;
  }
  clox::VirtualMachine VM;
  VM.init();
  auto result = VM.interpret(&chunk);
  return result;
}

static void repl() {
  char line[1024];
  while (true) {
    printf("> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static char *readFile(const char *path) {
  std::ifstream fin;
  fin.open(path, std::ios::in);
  if (!fin)
    return nullptr;
  fin.seekg(0, std::ios::end);
  size_t length = fin.tellg();
  fin.seekg(0, std::ios::beg);
  char *source = new char[length + 1];
  fin.read(source, length);
  return source;
}

static void runFile(const char *path) {
  char *source = readFile(path);
  if (source == nullptr) {
    fprintf(stderr, "Could not open file \"$%s\".\n", path);
    exit(74);
  }
  clox::InterpretResult result = interpret(source);

  delete source;
  if (result == clox::InterpretResult::INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == clox::InterpretResult::INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, char **argv) {
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }

  return 0;
}
