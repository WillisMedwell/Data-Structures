cmake_minimum_required(VERSION 3.7.2)
project(DataStructures)

set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.cxx")
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
file(GLOB_RECURSE BENCHMARK_SOURCES "benchmarks/*.cpp")
add_executable(Benchmark ${BENCHMARK_SOURCES})
target_link_libraries(Benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)
target_include_directories(
    Benchmark
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# GTest setup
find_package(GTest CONFIG REQUIRED)
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(Test ${TEST_SOURCES})
target_link_libraries(Test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_include_directories(
    Test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

