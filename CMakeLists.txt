cmake_minimum_required(VERSION 3.20)
project(jlox)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} 
  Main.cpp
  Scanner.hpp
  Scanner.cpp 
  Context.hpp
  Context.cpp)