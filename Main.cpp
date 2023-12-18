#include <fstream>
#include <iostream>
#include "Context.hpp"
#include "Scanner.hpp"
#include "Token.hpp"

static bool hadError{false};
static const char* File{nullptr};

static void Report(int line, const std::string &where,
                   const std::string &message) {
  std::cerr << "[Line " << line << "] Error" << where << ": " << message
            << std::endl;
  hadError = true;
}

static void Error(int line, const std::string &message) {
  Report(line, "", message);
}

int main(int argc, char **argv) { 
  if (argc == 1) {
    std::cout << "Usage : jlox [script]" << std::endl;
    return -1;
  }
  File = argv[1];
  std::cout << "Start compile " << File << std::endl;

  jlox::Context context;
  if (!context.readFromFile(File)) {
    std::cerr << "Cannot read " << File << std::endl;
    return -1;
  }

  jlox::Scanner sc{&context};
  sc.run();

  return 0;
}