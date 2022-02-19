cmake_minimum_required(VERSION 3.21)
project(glyph_lib C)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/glyphs.c src/example/example.c)

add_executable(glyph_lib_test ${SOURCE_FILES})
