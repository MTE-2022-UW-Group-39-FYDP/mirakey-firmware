cmake_minimum_required(VERSION 3.21)
project(mirakey_firmware C)

set(CMAKE_C_STANDARD 11)

add_executable(mirakey_firmware
        lib/glyphs/src/glyphs.c)
