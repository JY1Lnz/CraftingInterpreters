cmake_minimum_required(VERSION 3.20)
project(clox)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} 
  Main.cpp
  Common.hpp
  Chunk.hpp
  Chunk.cpp
  Debug.hpp
  Debug.cpp
  VirtualMachine.hpp
  VirtualMachine.cpp
  Value.hpp
  Value.cpp
  Compiler.hpp
  Compiler.cpp
  Scanner.hpp
  Scanner.cpp
  Token.hpp
  Token.cpp
  )