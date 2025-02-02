# BBEngine Baseball Simulation Engine

This is a realistic 3D baseball simulation engine built in modern C++ with plans for integration into Unreal Engine 5. The engine is a standalone, cross-platform library built using CMake. It tracks detailed player performance by recording comprehensive attributes and statistics over multiple seasons.

## Features

- Detailed player attributes for pitching, hitting, and fielding.
- Aggregates multiple seasons of player statistics.
- Modular architecture for independent development, testing, and future UE5 integration.
- Cross-platform support (Windows, Linux, macOS) using CMake.

## Directory Structure

BaseballEngine/
  CMakeLists.txt        # Build configuration file
  include/              # Header files
      BBEnginePlayerAttributes.h
      BBEnginePlayerStats.h
      BBEnginePlayer.h
  src/                  # Source files
      BBEnginePlayerAttributes.cpp
      BBEnginePlayerStats.cpp
      BBEnginePlayer.cpp
  tests/                # Test code
      main.cpp

## Building the Project

1. Clone the repository:
   git clone https://github.com/splanck/bbengine.git

2. Create a build directory and run CMake:
   cd baseball-engine
   mkdir build
   cd build
   cmake ..
   cmake --build .

3. Run the test executable in the tests/ directory to verify the setup.

## Usage

The core simulation logic is designed to integrate with Unreal Engine 5 via adapter classes that expose functionality to Blueprints and Unreal’s gameplay framework.

## License

Don't even think about it!