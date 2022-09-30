cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(TreeExplorer)

file(GLOB srcs *.h *.cpp *.hpp *qrc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(TreeExplorer_VERSION_MAJOR 1)
set(TreeExplorer_VERSION_MINOR 0)
set(TreeExplorer_VERSION_PATCH 1)
set(TreeExplorer_VERSION
  "${TreeExplorer_VERSION_MAJOR}.${TreeExplorer_VERSION_MINOR}.${TreeExplorer_VERSION_PATCH}")
add_definitions( -DTreeExplorer_MAJOR=${TreeExplorer_VERSION_MAJOR} )
add_definitions( -DTreeExplorer_MINOR=${TreeExplorer_VERSION_MINOR} )
add_definitions( -DTreeExplorer_PATCH=${TreeExplorer_VERSION_PATCH} )

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/icons/TreeExplorer.rc)
endif()


find_package(Qt5Widgets REQUIRED)

add_executable(TreeExplorer ${srcs} ${WIN32_RESOURCES})

target_link_libraries(TreeExplorer Qt5::Widgets)
