CMAKE_MINIMUM_REQUIRED(VERSION "2.8")
PROJECT("TIMSORT" "CXX")

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
                      "${CMAKE_MODULE_PATH}")

IF(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    MESSAGE(STATUS "CMAKE_BUILD_TYPE not set, defaulting to 'Debug'")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

INSTALL(
    FILES "include/timsort/timsort.h"
    DESTINATION "include/timsort")

ADD_EXECUTABLE(simple-test
    "test/simple-test.cpp")

TARGET_COMPILE_OPTIONS(simple-test
    PUBLIC "-std=c++14" "-Wall" "-Wextra" "-O3"
    "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-g>"
    "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-ggdb>"
    "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-UNDEBUG>"
    "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-fno-omit-frame-pointer>"
    "$<$<STREQUAL:$<CONFIGURATION>,Release>:-fomit-frame-pointer>"
    "$<$<STREQUAL:$<CONFIGURATION>,Release>:-DNDEBUG>")

TARGET_INCLUDE_DIRECTORIES(simple-test
    PUBLIC "include")

FIND_PACKAGE(Benchmark)

IF(BENCHMARK_FOUND)
    ADD_EXECUTABLE(benchmark
        "test/benchmark.cpp")

    TARGET_COMPILE_OPTIONS(benchmark
        PUBLIC "-std=c++14" "-Wall" "-Wextra" "-O3"
        "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-g>"
        "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-ggdb>"
        "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-UNDEBUG>"
        "$<$<STREQUAL:$<CONFIGURATION>,Debug>:-fno-omit-frame-pointer>"
        "$<$<STREQUAL:$<CONFIGURATION>,Release>:-fomit-frame-pointer>"
        "$<$<STREQUAL:$<CONFIGURATION>,Release>:-DNDEBUG>")

    TARGET_INCLUDE_DIRECTORIES(benchmark
        PUBLIC "include"
        PRIVATE "${BENCHMARK_INCLUDE_DIR}")

    TARGET_LINK_LIBRARIES(benchmark
        "${BENCHMARK_LIBRARY}"
        pthread)
ENDIF()
