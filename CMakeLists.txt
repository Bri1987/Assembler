cmake_minimum_required(VERSION 3.14)
project(as_final C)

set(CMAKE_C_STANDARD 11)
file(GLOB_RECURSE DIR_AS ./as/*.c)
file(GLOB_RECURSE DIR_LIB ./lib/*.c)
file(GLOB_RECURSE DIR_LINK ./link/*.c)

include_directories(.)
include_directories(as)
include_directories(lib)
include_directories(link)

add_executable(as_final main.c ${DIR_AS} ${DIR_LIB} ${DIR_LINK})
