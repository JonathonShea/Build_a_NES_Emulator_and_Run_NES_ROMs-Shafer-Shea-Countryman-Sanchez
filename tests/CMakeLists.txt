include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
enable_testing()
add_test(NAME example_test COMMAND nes_tests)
add_executable(nes_tests Run_Tests.cpp
              Cpu_Instruction_tests.cpp)
target_link_libraries(nes_tests gtest_main)
target_link_libraries(nes_tests MemoryMapper)


# The following variable is defined only on DLL systems
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
            TARGET nes_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:nes_tests> $<TARGET_FILE_DIR:nes_tests>
            COMMAND_EXPAND_LISTS
    )
endif ()
include(GoogleTest)
gtest_discover_tests(nes_tests)