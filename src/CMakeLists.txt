cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(TreeExplorer)

file(GLOB srcs *.h *.cpp *.hpp *qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(TreeExplorer ${srcs})

target_link_libraries(TreeExplorer Qt5::Widgets)