#include <stdio.h>
#include <stdlib.h>
#include "fileHelpers.h"

char* directoryLocation;
char* logLocation;
char* databaseLocations;

char *getServerDirectoryLocation(){
    return directoryLocation;
}

char *getLogLocation() {
    return logLocation;
}

char* getDatabaseLocation() {
    return databaseLocations;
}

int getDirectoryLocationFromUser() {

    printf("Setting up Server\n");

    for(int i = 0; i < 3; i++) {
        if(i == 0) {
            printf("Please enter the location of where you want the Server Directory to be: ");
        }else if(i == 1) {
            printf("Please enter the location of where you want the ServerLog to be: ");
        }else {
            printf("Please enter the location of where you want the Server Database to be: ");
        }

        size_t buffer_size = 128;
        size_t position = 0;
        char* buffer = malloc(buffer_size);
        if(buffer == NULL) {
            fprintf(stderr, "Unable to allocate memory to the buffer");
            return -1;
        }


        while(1) {
            int ch = getchar();
            if(ch == EOF || ch == '\n') {
                buffer[position] = '\0';
                break;
            }
            buffer[position++] = (char)ch;

            if(position >= buffer_size) {
                buffer_size *= 2;
                char *new_buffer = realloc(buffer, buffer_size);
                if(new_buffer == NULL) {
                    free(buffer);
                    fprintf(stderr, "Unable to allocate memeory to New Buffer");
                    return -1;
                }
                buffer = new_buffer;
            }
        }
        if(i== 0) {
            directoryLocation = buffer;
        }else if(i == 1) {
            logLocation = buffer;
        }else {
            databaseLocations = buffer;
        }
        printf("\n");
    }
    return 0;
}
