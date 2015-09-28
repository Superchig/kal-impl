cmake_minimum_required(VERSION 2.6)
# Kaleidoscope tutorial
project(LLMV_Compiler_Tutorial)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -pedantic -Wextra -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")

add_executable(kal_driver src/driver.cpp
  src/ast.cpp
  src/lexer.cpp
  src/parser.cpp)
