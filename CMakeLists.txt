cmake_minimum_required(VERSION 3.28)
project(BartmossCloud)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(BartmossCloud main.cpp
        loginwindow.cpp
        loginwindow.h
        mainwindow.h
        mainwindow.cpp
        c/directory.c
)
target_link_libraries(BartmossCloud
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

