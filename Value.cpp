#include "Value.hpp"
#include <cstdio>

using namespace clox;

Value::Value() : type(Type::NIL) {}

Value::Value(bool value) : boolean{value}, type(Type::BOOL) {}

Value::Value(double value) : number{value}, type(Type::NUMBER) {}

void clox::printValue(Value value) {
  switch (value.getType()) {
  case Value::Type::BOOL:
    printf(value.getBool() ? "true" : "false");
    break;
  case Value::Type::NIL:
    printf("nil");
    break;
  case Value::Type::NUMBER:
    printf("%g", value.getNumber());
    break;
  }
}
