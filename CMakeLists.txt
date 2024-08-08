cmake_minimum_required(VERSION 3.14)

project(WebSocket LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Network WebSockets REQUIRED)

set(PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ObjectManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Server.h
)

set( TARGET_SOURCE_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/source
)

add_executable(WebSocket ${PROJECT_SOURCES} ${TARGET_SOURCE_DIRS})
target_include_directories( WebSocket PRIVATE ${TARGET_SOURCE_DIRS} )
target_compile_definitions( WebSocket PRIVATE -DDISABLE_STATIC_HASH )

target_link_libraries(WebSocket Qt5::Core Qt5::WebSockets Qt5::Network)
