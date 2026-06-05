cmake_minimum_required(VERSION 3.16)
project(BoneTumourApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF) 
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

add_executable(BoneTumourApp
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

target_link_libraries(BoneTumourApp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)