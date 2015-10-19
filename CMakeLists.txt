cmake_minimum_required(VERSION 2.8)

# Set project name
set(PROJECT_NAME mcrt)
project(${PROJECT_NAME})

# Set include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)

# Store the source files in the SOURCES variable
file(GLOB SOURCES "src/*.cpp" "include/*.h")

# Add executable target to be built
add_executable(mcrt ${SOURCES})

# Require c++11 and debugging tools
set(CMAKE_CXX_FLAGS "-std=c++11 -g")