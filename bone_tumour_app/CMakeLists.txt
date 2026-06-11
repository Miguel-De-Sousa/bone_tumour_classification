cmake_minimum_required(VERSION 3.16)
project(BoneTumourApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF) 
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/onnxruntime/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/onnxruntime/lib)

add_executable(BoneTumourApp
    src/main.cpp
    src/analysisdashboard.cpp
    src/analysisdashboard.h
    src/bonetumourclassifier.h
    src/bonetumourclassifier.cpp
)

target_link_libraries(BoneTumourApp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui onnxruntime)
configure_file(model/bonetumour_model.onnx ${CMAKE_BINARY_DIR}/bonetumour_model.onnx COPYONLY)