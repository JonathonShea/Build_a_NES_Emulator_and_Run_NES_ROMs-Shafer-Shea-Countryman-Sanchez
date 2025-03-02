
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Now simply link against gtest or gtest_main as needed. Eg
enable_testing()
find_package(GTest CONFIG REQUIRED)
    enable_testing()
    

    add_test(AllTestsInMain main)
add_test(NAME example_test COMMAND nes_tests)
add_executable(nes_tests Run_Tests.cpp
              Cpu_Instruction_tests.cpp)
target_link_libraries(nes_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main NES)


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