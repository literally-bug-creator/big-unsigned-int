cmake_minimum_required(VERSION 3.21)
project(big_unsigned_int VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_subdirectory(big_unsigned_int)

if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "This project requires Clang compiler. Current: ${CMAKE_CXX_COMPILER_ID}")
    endif()

    message(STATUS "Building ${PROJECT_NAME} as the main project with Clang")

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        include(CheckIPOSupported)
        check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
        if(ipo_supported)
            set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
            message(STATUS "LTO enabled for Release build")
        else()
            message(STATUS "LTO not supported: ${ipo_error}")
        endif()
    endif()

    # Clang-specific compiler flags function
    function(add_clang_flags target_name)
        target_compile_options(${target_name} PRIVATE
            # Common flags
            -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
            -Wcast-qual -Wformat=2 -Wundef -Werror=return-type

            # Debug: No optimizations, debug info, runtime checks
            $<$<CONFIG:Debug>:-g -O0 -DDEBUG -fno-omit-frame-pointer -fno-optimize-sibling-calls>

            # Release: Maximum optimizations for performance
            $<$<CONFIG:Release>:-O3 -DNDEBUG -march=native -mtune=native
            -funroll-loops -fvectorize -fslp-vectorize -ffast-math -fno-signed-zeros
            -fno-trapping-math -fassociative-math -freciprocal-math -ffinite-math-only
            -fomit-frame-pointer -pipe>
        )

        target_link_options(${target_name} PRIVATE
            $<$<CONFIG:Release>:-Wl,--gc-sections -Wl,--strip-all>
        )
    endfunction()

    # CPU-specific optimizations for Release builds only
    include(CheckCXXCompilerFlag)

    # Check for additional CPU features
    check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
    check_cxx_compiler_flag("-mavx512f" COMPILER_SUPPORTS_AVX512F)
    check_cxx_compiler_flag("-mbmi2" COMPILER_SUPPORTS_BMI2)

    function(add_cpu_optimizations target_name)
        # Conservative CPU optimizations - only use AVX2 (widely supported)
        if(COMPILER_SUPPORTS_AVX2)
            target_compile_options(${target_name} PRIVATE
                $<$<CONFIG:Release>:-mavx2>
            )
            set(CPU_FEATURES "AVX2" PARENT_SCOPE)
        endif()

        # BMI2 is generally safe with modern processors
        if(COMPILER_SUPPORTS_BMI2)
            target_compile_options(${target_name} PRIVATE
                $<$<CONFIG:Release>:-mbmi2>
            )
        endif()
    endfunction()

    # Apply Clang flags to our main target
    add_clang_flags(big_unsigned_int)
    add_cpu_optimizations(big_unsigned_int)

    # Enable testing and add subdirectories
    enable_testing()
    add_subdirectory(tests)

    # Check if benchmarks directory exists
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/CMakeLists.txt")
        add_subdirectory(benchmarks)
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/CMakeLists.txt")
        add_subdirectory(benchmark)
    endif()

    message(STATUS "=== CLANG BUILD CONFIGURATION ===")
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(STATUS "Debug mode: No optimizations, full debug info")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        message(STATUS "Release mode: Maximum optimizations")
        message(STATUS "LTO enabled: ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}")
        message(STATUS "Target architecture: native")
        if(DEFINED CPU_FEATURES)
            message(STATUS "CPU features: ${CPU_FEATURES}")
        endif()
        if(COMPILER_SUPPORTS_BMI2)
            message(STATUS "BMI2 instructions: enabled")
        endif()
    endif()

    message(STATUS "=====================================")
else()
    message(STATUS "${PROJECT_NAME} is being included as a module")
endif()
