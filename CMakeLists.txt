cmake_minimum_required(VERSION 3.10)
project(HardDriveManager)

# Set global C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally, define global include directories or compile options here

# Add the backend directory
add_subdirectory(backend)

# If you plan to use CMake for additional modules (e.g., tests), add them here too
# add_subdirectory(tests)
