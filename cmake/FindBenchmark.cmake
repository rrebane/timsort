# CMake script for finding the Google Benchmark library
#
# This script sets the following variables:
#  BENCHMARK_FOUND          - Boolean that indicates if the package was found
#  BENCHMARK_INCLUDE_DIR    - Path to header files
#  BENCHMARK_LIBRARY        - The benchmark library

INCLUDE(FindPackageHandleStandardArgs)

FIND_PATH(
    BENCHMARK_INCLUDE_DIR
    NAMES
        benchmark/benchmark.h
    HINTS
        $ENV{BENCHMARK_ROOT}
        ${BENCHMARK_ROOT}
    PATHS
        /usr/local
        /usr
        /opt/local
    PATH_SUFFIXES
        include
)

FIND_LIBRARY(
    BENCHMARK_LIBRARY
    NAMES
        benchmark
    HINTS
        $ENV{BENCHMARK_ROOT}
        ${BENCHMARK_ROOT}
    PATHS
        /usr/local
        /usr
        /opt/local
    PATH_SUFFIXES
        lib
        lib64
        lib32
)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(BENCHMARK
    DEFAULT_MSG BENCHMARK_INCLUDE_DIR BENCHMARK_LIBRARY)
