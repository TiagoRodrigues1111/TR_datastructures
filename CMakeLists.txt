# ------------------------------------------------------------------
# CMakeLists.txt
# Root build file for tr_datastructures
# ------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)

project(
    tr_datastructures
    VERSION 0.1.0
    DESCRIPTION "A generic C data structures library"
    LANGUAGES C
)

# ------------------------------------------------------------------
# 1. PROJECT SETTINGS
# ------------------------------------------------------------------
set(CMAKE_C_STANDARD 99)          # minimum CMake-recognised standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ------------------------------------------------------------------
# 2. OPTIONS
# ------------------------------------------------------------------
option(TR_BUILD_TESTS      "Build the test suite"   OFF)
option(TR_BUILD_BENCHMARKS "Build the benchmarks"   OFF)
option(TR_BUILD_EXAMPLES   "Build the examples"     OFF)
option(TR_BUILD_FUZZ       "Build the fuzz targets" OFF)

# ------------------------------------------------------------------
# 3. COMPILER WARNINGS
# ------------------------------------------------------------------
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic              # this will warn on C89 incompatibilities
        -Wshadow
        -Wcast-align
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wconversion
        -Wsign-conversion
    )
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
endif()


# ------------------------------------------------------------------
# 4. SUBDIRECTORIES
# ------------------------------------------------------------------
include(GNUInstallDirs)

configure_file(
    cmake/tr_version.h.in
    include/tr_datastructures/tr_version.h
)

add_subdirectory(src)

if(TR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(TR_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(TR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(TR_BUILD_FUZZ)
    add_subdirectory(fuzz)
endif()

# ------------------------------------------------------------------
# 5. INSTALL & PACKAGING
# ------------------------------------------------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/tr_datastructures-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    cmake/tr_version.h.in
    include/tr_datastructures/tr_version.h
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/tr_datastructures-config-version.cmake"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tr_datastructures-config.cmake.in"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/tr_datastructures"
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/include/tr_datastructures/tr_version.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/tr_datastructures"
)