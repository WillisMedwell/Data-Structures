cmake_minimum_required(VERSION 3.7.2)
project(DataStructures)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 20)

# Just annoying.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-template-keyword")
elsif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.cxx")
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp" "tests/*.c" "test/*.cxx")
file(GLOB_RECURSE BENCHMARK_SOURCES "benchmarks/*.cpp" "benchmarks/*.c" "benchmarks/*.cxx")
file(GLOB         HEADERS "include/*.hpp" "include/*.h")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Make the start-up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})



# Google Benchmark setup
find_package(benchmark CONFIG REQUIRED)
add_executable(Benchmark ${BENCHMARK_SOURCES})
target_link_libraries(Benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(
    Benchmark
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# GTest setup
find_package(GTest CONFIG REQUIRED)
add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_include_directories(
    Test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

