cmake_minimum_required(VERSION 3.10)

# Set the project name
set(PROJECT_NAME SDSGC-information-tool)
project(${PROJECT_NAME})

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include all .cpp files
file(GLOB SOURCES "src/*.cpp")

# Add an executable
add_executable(${PROJECT_NAME} ${SOURCES})
