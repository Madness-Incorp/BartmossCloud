//
// Created by Oisin Lynch on 02/11/2024.
//

#include "dealwithDB.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "fileHelpers.h"
#include "fileLocationFunctions.h"
#include "logging.h"


int testConnection(const int socketID) {
    const char* fifoPath = getFIFOLocation();
    const int fifoid = open(fifoPath, O_RDONLY);

    int result = 0;
    if(read(socketID, &result, sizeof(int)) == 0) {
        perror("Error reading from the server");
        return -1;
    }

    if(write(fifoid, &result, sizeof(int)) == 0) {
        perror("Error writing to FIFO");
        return -1;
    }

    printf("Test connection complete\n");
    if (result == -1) {
        writeToLog("Connection with Server Unsuccessful");
    }else {
        writeToLog("Connection with Server Database Successful");
    }

    close(fifoid);
    return result;
}

int sendUsernamePasswordData(const int socketID, const char mode) {

    printf("Start reading user data!!!\n");
    const char* fifoPath = getFIFOLocation();
    const int fifoid = open(fifoPath, O_RDONLY);
    if(fifoid < 0) {
        perror("Error opening FIFO");
        return -1;
    }
    size_t usernameSize = 0, passwordSize = 0;
    char* Username;
    char* Password;

    //Read in Username Length, Password Length, Username and Password
    if(read(fifoid, &usernameSize, sizeof(size_t)) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(read(fifoid, &passwordSize, sizeof(size_t)) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(read(fifoid, &Username, usernameSize) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(read(fifoid, &Password, passwordSize) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    close(fifoid);

    printf("Read in Account Data: %s, %s\n", Username, Password);


    //Write the username length, password length, username and password
    if(write(socketID, &usernameSize, sizeof(size_t)) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(write(socketID, &passwordSize, sizeof(size_t)) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(write(socketID, &Username, usernameSize) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(write(socketID, &Password, passwordSize) == 0) {
        perror("Error reading from fifo");
        return -1;
    }

    if(mode == 'C') {
        int correctUsernamePassword = 0;
        if(read(socketID, &correctUsernamePassword, sizeof(int)) == 0) {
            perror("Error reading from fifo");
            return -1;
        }
        if(correctUsernamePassword == 1) {
            return 1;
        }
        return 3;
    }

    printf("Done\n");
    return 0;
}

int sendAccountData(const int socketID) {
    const char* fifoPath = getFIFOLocation();
    const int fifoid = open(fifoPath, O_RDONLY);
    if (fifoid < 0) {
        perror("Error opening FIFO");
        return -1;
    }

    size_t usernameSize, passwordSize;

    // Read the username and password sizes
    if (read(fifoid, &usernameSize, sizeof(size_t)) <= 0) {
        perror("Error reading username size");
        close(fifoid);
        return -1;
    }
    if (read(fifoid, &passwordSize, sizeof(size_t)) <= 0) {
        perror("Error reading password size");
        close(fifoid);
        return -1;
    }

    // Allocate memory for username and password
    char *Username = malloc(usernameSize + 1);
    char *Password = malloc(passwordSize + 1);
    if (Username == NULL || Password == NULL) {
        perror("Memory allocation error");
        close(fifoid);
        free(Username);
        free(Password);
        return -1;
    }

    // Read the Username
    if (read(fifoid, Username, usernameSize) <= 0) {
        perror("Error reading Username");
        close(fifoid);
        free(Username);
        free(Password);
        return -1;
    }
    Username[usernameSize] = '\0';  // Null-terminate the string

    // Read the Password
    if (read(fifoid, Password, passwordSize) <= 0) {
        perror("Error reading Password");
        close(fifoid);
        free(Username);
        free(Password);
        return -1;
    }
    Password[passwordSize] = '\0';  // Null-terminate the string

    // Close the FIFO
    close(fifoid);

    // Display the Username and Password
    printf("Read in Account Data: Username: %s, Password: %s\n", Username, Password);

    // Send username size, password size, username, and password
    if (write(socketID, &usernameSize, sizeof(size_t)) < 0 ||
        write(socketID, &passwordSize, sizeof(size_t)) < 0 ||
        write(socketID, Username, usernameSize) < 0 ||
        write(socketID, Password, passwordSize) < 0) {
        perror("Error writing to cfd");
        free(Username);
        free(Password);
        return -1;
    }

    printf("Data written successfully to cfd\n");

    // Clean up
    free(Username);
    free(Password);
    return 0;
}
