cmake_minimum_required(VERSION 3.28)
project(BartmossCloud)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)

# Enable Qt's automoc, autorcc, and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Define the Client executable
add_executable(Client
        MainClient.c
        ClientConstruct.c
        directory.c
        logging.c
        receiving.c
        sending.c
        dealwithDB.c
        importantFileLocations.c
)

# Define the BartmossCloud executable
add_executable(BartmossCloud
        main.cpp
        loginwindow.cpp
        mainwindow.cpp
        directForGui.c
        logging.c
        LinkedList.cpp
        createAccountWindow.cpp
        editAccountDatabase.cpp
        importantFileLocations.c
)

add_executable(SetUpPage runSetup.cpp setuppage.cpp importantFileLocations.c logging.c)

# Link the BartmossCloud executable with the Qt libraries
target_link_libraries(BartmossCloud
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(SetUpPage
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Ensure multiThreading runs Client and BartmossCloud
add_executable(multiThreading multiThreading.cpp importantFileLocations.c logging.c
        makeFIFO.cpp
        makeFIFO.h makeFIFO.cpp)

# Link the multiThreading executable with the Qt libraries
target_link_libraries(multiThreading
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

# Link the multiThreading executable with Client and BartmossCloud
add_dependencies(multiThreading Client BartmossCloud SetUpPage)
