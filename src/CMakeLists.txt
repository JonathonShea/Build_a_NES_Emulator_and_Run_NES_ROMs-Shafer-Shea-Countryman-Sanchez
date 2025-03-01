set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(NES SHARED 
         "NesRam.h" "NesRam.cpp" "Cartridge.h" "Cartridge.cpp" "Clock.h" "Clock.cpp" "Utilities.h" "Utilities.cpp"
        CPU.h CPU.cpp "event/EventDispatcher.h" "event/EventDispatcher.cpp" "event/IEvent.h" "event/Event.h" "Mapper0.h" "Mapper0.cpp")
add_library(CPU SHARED
            CPU.h CPU.cpp "event/EventDispatcher.h" "event/EventDispatcher.cpp" "event/IEvent.h" "event/Event.h" "Mapper0.h" "Mapper0.cpp")