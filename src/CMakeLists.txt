set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(MemoryMapper SHARED MemoryMapper.cpp
        MemoryMapper.h "NesRam.h" "NesRam.cpp" "Cartridge.h" "Cartridge.cpp" "Clock.h" "Clock.cpp" "Utilities.h" "Utilities.cpp"
        CPU.h CPU.cpp)
add_library(CPU SHARED
            CPU.h CPU.cpp)