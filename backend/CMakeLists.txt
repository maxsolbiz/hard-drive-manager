cmake_minimum_required(VERSION 3.10)
project(HardDriveManager)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add FetchContent for nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2  # or update to your desired version
)
FetchContent_MakeAvailable(json)

# Collect all source files in the src folder
file(GLOB_RECURSE BACKEND_SOURCES "src/*.cpp")

# Exclude main.cpp from the library sources
list(FILTER BACKEND_SOURCES EXCLUDE REGEX "src/main.cpp")

# Create a static library for all backend sources (except main.cpp)
add_library(hard_drive_manager_lib ${BACKEND_SOURCES})

# Include header files directory
target_include_directories(hard_drive_manager_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link Threads library
find_package(Threads REQUIRED)
target_link_libraries(hard_drive_manager_lib Threads::Threads)

# Link SQLite3 library for logging/reporting functionality
find_library(SQLITE3_LIBRARY sqlite3)
if(SQLITE3_LIBRARY)
    target_link_libraries(hard_drive_manager_lib ${SQLITE3_LIBRARY})
else()
    message(FATAL_ERROR "SQLite3 library not found!")
endif()

# Use pkg-config to find JsonCpp (if needed)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})
target_link_libraries(hard_drive_manager_lib ${JSONCPP_LIBRARIES})

# Create the main executable from main.cpp only
add_executable(hard_drive_manager src/main.cpp)
target_link_libraries(hard_drive_manager hard_drive_manager_lib)

# Create a standalone executable for full SMART details (health_monitor_full)
add_executable(health_monitor_full src/health_monitor_full.cpp)
target_link_libraries(health_monitor_full hard_drive_manager_lib)
target_include_directories(health_monitor_full PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

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
    target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(runTests hard_drive_manager_lib ${GTEST_LIBRARIES} Threads::Threads ${JSONCPP_LIBRARIES})
    add_test(NAME runTests COMMAND runTests)
endif()

# Build the Health Monitoring test executable
add_executable(health_monitor_main src/health_monitor_main.cpp)
target_link_libraries(health_monitor_main hard_drive_manager_lib)
