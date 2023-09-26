cmake_minimum_required(VERSION 3.14)
project(as_final C)

set(CMAKE_C_STANDARD 11)
file(GLOB_RECURSE DIR_AS ./as/*.c)

include_directories(.)
include_directories(as)

add_executable(as_final main.c ${DIR_AS})
