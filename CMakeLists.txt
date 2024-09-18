cmake_minimum_required(VERSION 3.13)

# Set the project name and language
set(PROJECT_NAME sdsgc-information-tool)
project(${PROJECT_NAME} CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use all .cpp files as source code to build an application
file(GLOB SOURCES "src/*.cpp")
option(WARN "Enable additional compiler warnings" OFF)
if(WARN)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()
add_executable(${PROJECT_NAME} ${SOURCES})

option(STATIC "Use static linking to include libraries in the executable file" ON)
if(STATIC)
    target_link_libraries(${PROJECT_NAME} -static)
endif()

option(BUILD_INSTALLER "Package files in the correct directory structure for Windows installer" OFF)
if(BUILD_INSTALLER)
    include(build/Installer.cmake)
endif()
