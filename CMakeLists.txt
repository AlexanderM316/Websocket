cmake_minimum_required(VERSION 3.14)

project(WebSoket LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS WebSockets REQUIRED)

add_executable(WebSoket
  main.cpp
  Object.h
  ObjectManager.h
  Server.h
  Server.cpp
)
target_link_libraries(WebSoket Qt${QT_VERSION_MAJOR}::Core Qt5::WebSockets)
