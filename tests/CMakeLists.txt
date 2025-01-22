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
add_test(NAME example_test COMMAND example_tests)
add_executable(example_tests Run_Tests.cpp
        example_test.cpp "Cpu_Instruction_tests.cpp")
target_link_libraries(example_tests gtest_main)
target_link_libraries(example_tests MemoryMapper)


# The following variable is defined only on DLL systems
if (CMAKE_IMPORT_LIBRARY_SUFFIX)
    add_custom_command(
            TARGET example_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:example_tests> $<TARGET_FILE_DIR:example_tests>
            COMMAND_EXPAND_LISTS
    )
endif ()