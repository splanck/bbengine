cmake_minimum_required(VERSION 3.10)
project(BaseballEngine VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a library target from the source files
add_library(BaseballEngine
    src/Player.cpp
    src/Team.cpp
    src/League.cpp
    src/SimulationManager.cpp
    src/Game.cpp
)

# Specify the include directories for this library
target_include_directories(BaseballEngine PUBLIC
    \/include
)

# Optionally create a test executable
add_executable(BaseballEngineTest
    tests/main.cpp
)

# Link the library to the test executable
target_link_libraries(BaseballEngineTest PRIVATE BaseballEngine)
