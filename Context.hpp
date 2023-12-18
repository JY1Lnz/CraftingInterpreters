#pragma once
/// This file define the core context in jlox

#include <string>
#include <string_view>
namespace jlox {

class Context {
private:
  std::string text;

public:
  Context() = default;
  Context(const std::string &text) : text{text} {}

  bool readFromFile(const char* fileName);

  std::string_view getText() { return text; }
};

} // namespace jlox