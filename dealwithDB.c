//
// Created by Oisin Lynch on 02/11/2024.
//

#include "dealwithDB.h"

#include <unistd.h>
#include <sys/fcntl.h>

#include "fileHelpers.h"

char testConnection(const int socketID) {
    const int fifoid = open(FIFOPATH, O_RDONLY);
    char dbrequest = ' ';
    if(read(fifoid, &dbrequest, sizeof(char)) == 0) {
        perror("Error reading from fifo");
        return 'E';
    }

    if(write(socketID, &dbrequest, sizeof(char)) == 0) {
        perror("Error writing to server");
        return 'E';
    }

    int result = 0;
    if(read(socketID, &result, sizeof(int)) == 0) {
        perror("Error reading from the server");
        return 'E';
    }

    if(write(fifoid, &result, sizeof(int)) == 0) {
        perror("Error writing to FIFO");
        return 'E';
    }

    printf("Test connection complete\n");

    close(fifoid);
    return dbrequest;
}

int sendUsernamePasswordData(const int socketID, const char mode) {

    printf("Start reading user data!!!\n");

    const int fifoid = open(FIFOPATH, O_RDONLY);
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
