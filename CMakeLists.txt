cmake_minimum_required(VERSION 3.30)
project(rusoku-wirechark-plugin C)

set(CMAKE_C_STANDARD 11)

# List all source files
set(SOURCES
        src/gopt.c
        src/gopt-errors.c
        src/main.c
        include/gopt.h
        src/main.c
)

add_executable(rusoku-wireshark-plugin ${SOURCES})
