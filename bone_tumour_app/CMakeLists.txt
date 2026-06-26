cmake_minimum_required(VERSION 3.16)
project(BoneTumourApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF) 
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui PrintSupport)

add_executable(BoneTumourApp
    src/main.cpp
    src/analysisdashboard.cpp
    include/analysisdashboard.h
    include/bonetumourclassifier.h
    src/bonetumourclassifier.cpp
    include/animatedstackedwidget.h
    src/animatedstackedwidget.cpp
    assets/assets.qrc
    include/reportinfodialog.h
    src/reportinfodialog.cpp
    include/editdiagnosisdialog.h
    src/editdiagnosisdialog.cpp
)

target_include_directories(BoneTumourApp PRIVATE 
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/onnxruntime/include
)

target_link_directories(BoneTumourApp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/onnxruntime/lib
)

target_link_libraries(BoneTumourApp PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui onnxruntime Qt6::PrintSupport)

configure_file(model/bonetumour_model.onnx ${CMAKE_BINARY_DIR}/bonetumour_model.onnx COPYONLY)