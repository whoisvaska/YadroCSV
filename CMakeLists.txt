cmake_minimum_required(VERSION 3.1)
project(YadroCSV)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(csvreader main.cpp table.cpp table.h string_helper.cpp string_helper.h)

