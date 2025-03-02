cmake_minimum_required(VERSION 3.26)
project(NES_Emulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify SDL2 directory where SDL2Config.cmake is located
set(SDL2_DIR "C:/SDL2/cmake")  # Path to the cmake directory containing SDL2Config.cmake
set(SDL2_LIBRARIES "C:/SDL2/lib/x64/SDL2.lib;C:/SDL2/lib/x64/SDL2main.lib")


# Find SDL2 package
#find_package(SDL2 REQUIRED)

add_executable(nes_emulator main.cpp)
target_link_libraries(nes_emulator PRIVATE NES)
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
            TARGET nes_emulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:nes_emulator> $<TARGET_FILE_DIR:nes_emulator>
            COMMAND_EXPAND_LISTS
    )
endif ()

add_executable(APU_Test ../src/APU.cpp)
target_include_directories(APU_Test PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(APU_Test PRIVATE ${SDL2_LIBRARIES})

if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
        TARGET APU_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:APU_Test> $<TARGET_FILE_DIR:APU_Test>
        COMMAND_EXPAND_LISTS
    )
endif()

add_executable(CPU_Test ../src/CPU.cpp)
target_include_directories(CPU_Test PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(CPU_Test PRIVATE ${SDL2_LIBRARIES})
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
        TARGET CPU_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:APU_Test> $<TARGET_FILE_DIR:APU_Test>
        COMMAND_EXPAND_LISTS
    )
endif()