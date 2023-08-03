# The minimum cmake version requirements
cmake_minimum_required(VERSION 3.7.2)

# via https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# The project Gridworld
project(DataStructures)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "src/*.cxx")
file(GLOB         HEADERS "include/*.hpp" "include/*.h")

# Create the solution for project from the SOURCES variable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# The directories included in the build command
target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Make the start up project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
