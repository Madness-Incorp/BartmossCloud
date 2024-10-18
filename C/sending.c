//
// Created by Oisin Lynch on 20/08/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fileHelpers.h"
#include <sys/fcntl.h>

void send_file(FILE * fp, const int sockID, const size_t filesize) {
    char * buffer = malloc(filesize);
    if (buffer == NULL) {
        fprintf(stderr, "Issue with memory allocation\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, filesize, fp);

    if (write(sockID, buffer, filesize) == -1) {
        fprintf(stderr, "Error with sending the file\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    free(buffer);
}

void sendMessage(const int socketID, const char* message) {

    const ssize_t bytesWritten = write(socketID, message, strlen(message));
    if(bytesWritten <= 0) {
        fprintf(stderr, "Error writing into socket");
        close(socketID);
        exit(EXIT_FAILURE);
    }

}

char* convertToDollarString(char* string) {

    char* newMessage = malloc((strlen(string) + 2) * sizeof(char));

    if (newMessage == NULL) {
        perror("malloc failed");
        free(string);
        return NULL;
    }

    newMessage[0] = '\0';
    strcat(newMessage, string);
    strcat(newMessage, "$");

    return newMessage;
}

int writeFIFO(const char* string) {

    const int FIFOId = open(fifopath, O_WRONLY);
    if(FIFOId == -1) {
        perror("Error opening FIFO");
        return -1;
    }

    const ssize_t bytesWritten = write(FIFOId, string, strlen(string));
    if(bytesWritten == 0) {
        perror("Error writing to FIFO");
        close(FIFOId);
        return -1;
    }

    printf("Files written to FIFO\n");

    close(FIFOId);
    return 0;
}
