cmake_minimum_required(VERSION 3.28)
project(BartmossCloud)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)

# Enable Qt's automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the required Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Define the Client executable
add_executable(Client
        client.c
        ClientConstruct.c
        directory.c
        logging.c
        receiving.c
        sending.c
        dealwithDB.c
        client.h
        dealwithDB.h
        fileHelpers.h
        logging.h
)

# Define the BartmossCloud executable
add_executable(BartmossCloud
        main.cpp
        loginwindow.cpp
        loginwindow.h
        mainwindow.cpp
        mainwindow.h
        directForGui.c
        LinkedList.cpp
        createAccountWindow.cpp
        createAccountWindow.h
        editAccountDatabase.cpp
        editAccountDatabase.h
)

# Link the BartmossCloud executable with the Qt libraries
target_link_libraries(BartmossCloud
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Define the multiThreading executable that launches both Client and BartmossCloud
add_executable(multiThreading multiThreading.c)

# Ensure multiThreading runs Client and BartmossCloud
add_dependencies(multiThreading Client BartmossCloud)
