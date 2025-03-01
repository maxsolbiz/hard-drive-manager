cmake_minimum_required(VERSION 3.10)
project(HardDriveManager)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files in the src folder
file(GLOB_RECURSE BACKEND_SOURCES "src/*.cpp")

# Exclude main.cpp from the library sources
list(FILTER BACKEND_SOURCES EXCLUDE REGEX "src/main.cpp")

# Create a static library for all backend sources (except main.cpp)
add_library(hard_drive_manager_lib ${BACKEND_SOURCES})

# Include header files directory
target_include_directories(hard_drive_manager_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link pthreads (or Threads library) if necessary
find_package(Threads REQUIRED)
target_link_libraries(hard_drive_manager_lib Threads::Threads)

# Link SQLite3 library for logging/reporting functionality
find_library(SQLITE3_LIBRARY sqlite3)
if(SQLITE3_LIBRARY)
    target_link_libraries(hard_drive_manager_lib ${SQLITE3_LIBRARY})
else()
    message(FATAL_ERROR "SQLite3 library not found!")
endif()

# Create the main executable from main.cpp only
add_executable(hard_drive_manager src/main.cpp)
target_link_libraries(hard_drive_manager hard_drive_manager_lib)

# Enable testing
enable_testing()

# Find GoogleTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Collect test source files
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
if(TEST_SOURCES)
    add_executable(runTests ${TEST_SOURCES})
    target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    # Also add the tests directory so that common helper headers are found.
    target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    # Link the library that contains the core modules, plus GTest and Threads
    target_link_libraries(runTests hard_drive_manager_lib ${GTEST_LIBRARIES} Threads::Threads)
    add_test(NAME runTests COMMAND runTests)
endif()

# Build the Health Monitoring test executable
add_executable(health_monitor_main src/health_monitor_main.cpp)
target_link_libraries(health_monitor_main hard_drive_manager_lib)

