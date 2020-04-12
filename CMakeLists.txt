cmake_minimum_required(VERSION 3.5)

project(UdpComm LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED Network REQUIRED)

add_executable(UdpComm
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
  res/res.qrc
)

target_link_libraries(UdpComm Qt5::Widgets Qt5::Network)
