#include "Context.hpp"
#include <fstream>

using namespace jlox;

bool Context::readFromFile(const char *fileName) {
  std::ifstream fin;
  fin.open(fileName, std::ios::in);
  if (!fin) {
    return false;
  }
  fin.seekg(0, std::ios::end);
  auto length = fin.tellg();
  fin.seekg(0, std::ios::beg);
  text.resize(length);
  fin.read(text.data(), length);
  return true;
}