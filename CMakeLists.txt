cmake_minimum_required(VERSION 3.5)

# Set the project name and language
set(PROJECT_NAME sdsgc-information-tool)
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use all .cpp files as source code to build an application
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
