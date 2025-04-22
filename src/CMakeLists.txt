set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(NES SHARED 
         "NesRam.h" "NesRam.cpp" "Cartridge.h" "Cartridge.cpp" "Clock.h" "Clock.cpp" "Utilities.h" "Utilities.cpp" "input.h" "input.cpp"
        CPU.h CPU.cpp PPU.h PPU.cpp)
         CPU.h CPU.cpp OAM.h)
add_library(CPU SHARED
            CPU.h CPU.cpp "Utilities.h" "Utilities.cpp" PPU.h PPU.cpp)

find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(NES
        PUBLIC
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )


# find_package(SDL2_mixer CONFIG REQUIRED)
# target_link_libraries(NES PUBLIC $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
