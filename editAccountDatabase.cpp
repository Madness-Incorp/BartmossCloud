#include "editAccountDatabase.h"
#include <iostream>
#include <ostream>
#include <utility>
#include <unistd.h>
#include <sys/fcntl.h>

extern "C"{
    const char* getFIFOLocation();
}


int editAccountDatabase::testConnection() {
    const char* fifoLocation = getFIFOLocation();
    const int fifoID = open(fifoLocation, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return -1;
    }
    printf("Before");
    int testResult = 0;
    if(read(fifoID, &testResult, sizeof(int)) == 0) {
        perror("Error reading from FIFO");
        return -1;
    }
    printf("After");

    printf("Results %d\n", testResult);

    if(testResult == -1) {
        std::cerr << "Error conecting to server dataBase" << std::endl;
        return -1;
    }

    close(fifoID);
    return 0;
}

int editAccountDatabase::sendAccountDetails(std::string username, std::string password) {
    const char* Cusername = username.c_str();
    const char* Cpassword = password.c_str();

    const size_t usernameSize = strlen(Cusername);
    const size_t passwordSize = strlen(Cpassword);

    const char* fifoLocation = getFIFOLocation();

    printf("%s %s %zu %zu\n", Cusername, Cpassword, usernameSize, passwordSize);

    const int fifoID = open(fifoLocation, O_WRONLY);
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

int editAccountDatabase::checkAccountDetailsorCreateAccount(std::string username, std::string password, int choice) {

    if(sendAccountDetails(std::move(username), std::move(password)) != 0) {
        perror("Error sending account details to client");
    }

    const char* location = getFIFOLocation();
    const int fifoID = open(location, O_WRONLY);
    if(fifoID == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    char operationSymbol;
    if(choice == 1) {
         operationSymbol = 'V';
    }else {
        operationSymbol = 'C';
    }

    if(write(fifoID, &operationSymbol, sizeof(char)) <= 0) {
        perror("Error writing operationSymbol into the FIFO");
        return -1;
    }
    close(fifoID);

    const int fifoID2 = open(location, O_RDONLY);
    if(fifoID2 == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    int resultofAccountCheck = 0;

    if(read(fifoID2, &resultofAccountCheck, sizeof(int)) <= 0) {
        perror("Error when reading resultofAccountCheck from FIFO");
        return -1;
    }
    printf("Read to FIFO is complete! %d\n", resultofAccountCheck);

    close(fifoID2);
    return resultofAccountCheck;
}