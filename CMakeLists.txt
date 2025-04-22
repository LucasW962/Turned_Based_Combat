cmake_minimum_required(VERSION 3.10)
project(TurnBasedBattle VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Include header files
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add the executable
add_executable(TurnBasedBattle ${SOURCES})

# Set output directory
set_target_properties(TurnBasedBattle
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# Add compiler warning flags
if(MSVC)
    target_compile_options(TurnBasedBattle PRIVATE /W4)
else()
    target_compile_options(TurnBasedBattle PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print status messages
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Project source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Project binary dir: ${CMAKE_BINARY_DIR}")