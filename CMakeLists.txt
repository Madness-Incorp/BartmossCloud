cmake_minimum_required(VERSION 3.28)
project(BartmossCloud)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)

# Enable Qt's automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the required Qt packages
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Define the C target
add_library(C_Library STATIC
        C/client.c
        C/ClientConstruct.c
        C/directory.c
        C/logging.c
        C/receiving.c
        C/sending.c
        C/LinkedList.c
)

# Define the C++/Qt target
add_executable(BartmossCloud
        main.cpp
        loginwindow.cpp
        loginwindow.h
        mainwindow.h
        mainwindow.cpp
        directForGui.c
)

# Link the C++/Qt executable with the Qt libraries
target_link_libraries(BartmossCloud
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Optionally, link the C library to the C++/Qt app if you need the C code to be used in the Qt app
target_link_libraries(BartmossCloud
        C_Library   # Link C executable/library to BartmossCloud if needed
)
