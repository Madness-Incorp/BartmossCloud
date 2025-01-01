//
// Created by Oisin Lynch on 01/11/2024.
//

#include "editAccountDatabase.h"
#include <iostream>
#include <ostream>
#include <utility>
#include <unistd.h>
#include <sys/fcntl.h>
#define FIFOPATH "/Users/oisin/CLionProjects/pipingTest/my_fifo"

int editAccountDatabase::testConnection(const bool connectionNeeded) {

    const int fifoID = open(FIFOPATH, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    char requestChar = ' ';

    if(connectionNeeded) {
        requestChar = 'T';
    }else {
        requestChar = 'N';
    }
    if(write(fifoID, &requestChar, sizeof(char)) <= 0) {
        perror("Error writing into the FIFO");
        return -1;
    }

    int testResult = 0;
    if(read(fifoID, &testResult, sizeof(int)) == 0) {
        perror("Error reading from FIFO");
        return -1;
    }

    printf("Results %d\n", testResult);

    if(testResult == -1) {
        std::cerr << "Error conecting to server dataBase" << std::endl;
        return 0;
    }

    close(fifoID);
    return 1;
}

int editAccountDatabase::sendAccountDetails(std::string username, std::string password) {
    const char* Cusername = username.c_str();
    const char* Cpassword = password.c_str();

    const size_t usernameSize = strlen(Cusername);
    const size_t passwordSize = strlen(Cpassword);

    printf("%s %s %zu %zu\n", Cusername, Cpassword, usernameSize, passwordSize);

    const int fifoID = open(FIFOPATH, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    //Send the length of the username then the password length
    if(write(fifoID, &usernameSize, sizeof(size_t)) <= 0) {
        perror("Error writing usernameSize into the FIFO");
        return -1;
    }

    printf("Written into FIFIO\n");

    if(write(fifoID, &passwordSize, sizeof(size_t)) <= 0) {
        perror("Error writing passwordSize into the FIFO");
        return -1;
    }

    //Send the username and password
    if(write(fifoID, &username, usernameSize) <= 0) {
        perror("Error writing Username into the FIFO");
        return -1;
    }

    if(write(fifoID, &password, passwordSize) <= 0) {
        perror("Error writing Paswword into the FIFO");
        return -1;
    }

    std::cout << "Sent Account data to Client process" << std::endl;
    close(fifoID);
    return 0;
}

int editAccountDatabase::checkAccountDetails(std::string username, std::string password) {

    if(sendAccountDetails(std::move(username), std::move(password)) != 0) {
        perror("Error sending account details to client");
    }

    const int fifoID = open(FIFOPATH, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    constexpr char verifySymbol = 'v';

    if(write(fifoID, &verifySymbol, sizeof(char)) <= 0) {
        perror("Error writing verifySymbol into the FIFO");
        return -1;
    }

    int resultofAccountCheck = 0;

    if(read(fifoID, &resultofAccountCheck, sizeof(int)) <= 0) {
        perror("Error when reading resultofAccountCheck from FIFO");
        return -1;
    }

    close(fifoID);
    return resultofAccountCheck;
}




