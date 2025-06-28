set(BENCHMARK_USE_BUNDLED_GTEST OFF)
set(BENCHMARK_ENABLE_TESTING OFF)

add_subdirectory(third-party/benchmark)

file(GLOB_RECURSE BENCHMARK_SOURCES "./benchmarks/*.cpp")
add_executable(run_benchmark ${BENCHMARK_SOURCES})

target_link_libraries(run_benchmark benchmark::benchmark big_unsigned_int)
target_include_directories(run_benchmark PRIVATE ${benchmark_SOURCE_DIR}/include)

# Apply Clang optimizations to benchmarks
if(COMMAND add_clang_flags)
    add_clang_flags(run_benchmark)
endif()

if(COMMAND add_cpu_optimizations)
    add_cpu_optimizations(run_benchmark)
endif()
