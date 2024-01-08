#pragma once

namespace clox {

class Obj;

class Value {
public:
  enum class Type {
    BOOL,
    NIL,
    NUMBER,
    OBJ,
  };

private:
  Type type;
  union {
    bool boolean;
    double number;
    Obj *obj;
  };

public:
  // static Value getBool(bool value);
  Value();
  explicit Value(bool value);
  explicit Value(double value);

  bool isBool() const { return type == Type::BOOL; }
  bool isNumber() const { return type == Type::NUMBER; }
  bool isNil() const { return type == Type::NIL; }
  bool isObj() const { return type == Type::OBJ; }

  double getNumber() const { return number; }
  bool getBool() const { return boolean; }
  Obj *getObj() const { return obj; }

  Type getType() const { return type; }
};

void printValue(Value value);
} // namespace clox