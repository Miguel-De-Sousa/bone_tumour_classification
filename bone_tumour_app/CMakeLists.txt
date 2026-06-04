cmake_minimum_required(VERSION 3.16)
project(BoneTumourApp VERSION 1.0 LANGUAGES CXX)

# Require C++17 or higher for modern Qt 6 features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Instruct CMake to run Qt's Meta-Object Compiler automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt 6 Core, Widgets, and Gui modules
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Define source code files
add_executable(BoneTumourApp
    src/main.cpp
)

# Link the Qt 6 libraries to the executable application
target_link_libraries(BoneTumourApp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)