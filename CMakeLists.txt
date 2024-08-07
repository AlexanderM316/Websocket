cmake_minimum_required(VERSION 3.14)

project(WebSocket LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network WebSockets REQUIRED)

add_executable(WebSocket
  main.cpp
  Object.h
  ObjectManager.h
  Server.h
  Server.cpp
)

target_link_libraries(WebSocket Qt5::Core Qt5::WebSockets Qt5::Network)
