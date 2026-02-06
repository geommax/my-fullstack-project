cmake_minimum_required(VERSION 3.10)
project(GrowthCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Add executable
add_executable(growth_calc main.cpp)

# Link math library
target_link_libraries(growth_calc m pthread)

# Installation
install(TARGETS growth_calc DESTINATION /usr/local/bin)

# Print build information
message(STATUS "")
message(STATUS "Growth Pattern Calculator - Build Configuration")
message(STATUS "================================================")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Output: growth_calc")
message(STATUS "================================================")
message(STATUS "")
