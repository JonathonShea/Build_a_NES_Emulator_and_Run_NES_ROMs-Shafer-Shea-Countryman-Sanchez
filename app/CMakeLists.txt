cmake_minimum_required(VERSION 3.26)
project(NES_Emulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(nes_emulator main.cpp)
target_link_libraries(nes_emulator PRIVATE NES)
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
            TARGET nes_emulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:nes_emulator> $<TARGET_FILE_DIR:nes_emulator>
            COMMAND_EXPAND_LISTS
    )
endif ()


