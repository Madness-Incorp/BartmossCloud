cmake_minimum_required(VERSION 3.28)
project(Cloud)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)

find_package(SQLite3 REQUIRED)


# Define the C target
add_executable(Server
        MainServer.c
        ServerConstruct.c
        sending.c
        receiving.c
        logging.c
        logging.h
        directory.c
        dataBase.c
        Encryption/SHA256.c
        getLocations.c
)

target_link_libraries(Server SQLite::SQLite3)
