cmake_minimum_required(VERSION 3.26)
project(nes_emulator)

set(CMAKE_CXX_STANDARD 20)
enable_testing()
include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)