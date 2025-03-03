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

add_executable(APU_Test ../src/APU.cpp)
find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(APU_Test
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
target_link_libraries(APU_Test PRIVATE SDL2::SDL2main)

if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
        TARGET APU_Test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:APU_Test> $<TARGET_FILE_DIR:APU_Test>
        COMMAND_EXPAND_LISTS
    )
endif()

#add_executable(CPU_Test ../src/CPU.cpp ../src/Utilities.cpp)
#if (CMAKE_IMPORT_LIBRARY_SUFFIX)
 #   add_custom_command(
  #      TARGET CPU_Test POST_BUILD
   #     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:APU_Test> $<TARGET_FILE_DIR:APU_Test>
    #    COMMAND_EXPAND_LISTS
    #)
#endif()