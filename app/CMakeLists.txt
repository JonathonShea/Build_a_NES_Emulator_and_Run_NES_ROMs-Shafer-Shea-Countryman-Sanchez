add_executable(nes_emulator main.cpp)
target_link_libraries(nes_emulator PRIVATE MemoryMapper)
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
            TARGET nes_emulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:nes_emulator> $<TARGET_FILE_DIR:nes_emulator>
            COMMAND_EXPAND_LISTS
    )
endif ()