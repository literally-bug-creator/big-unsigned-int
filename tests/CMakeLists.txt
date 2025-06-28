add_subdirectory(third-party/gtest)

include(GoogleTest)

file(GLOB_RECURSE TESTS_SRC_FILES tests/*.cpp)
add_executable(run_tests ${TESTS_SRC_FILES})

target_link_libraries(run_tests gtest gtest_main big_unsigned_int)
target_include_directories(run_tests PRIVATE ../big_unsigned_int/include)

enable_testing()
gtest_discover_tests(run_tests)
