#include <stdio.h>
#include "importantFileLocations.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "logging.h"
#include "fileLocationFunctions.h"

static enum FILE_LOCATION_STATUS file_directory_status = READY_TO_BE_SET;
static enum FILE_LOCATION_STATUS log_location_status = READY_TO_BE_SET;
static enum FILE_LOCATION_STATUS fifo_location_status = READY_TO_BE_SET;

int checkIfLocationsSet() {

    FILE *fileLocations = fopen("/Users/oisin/CLionProjects/BartmossCloud/fileLocations.txt", "r");

    if(fileLocations == NULL) {
        perror("Unable to obtain locations, ensure that fileLocations.txt has not been moved");
        exit(EXIT_FAILURE);
    }

    fseek(fileLocations, 0, SEEK_END);
    const size_t size = ftell(fileLocations);

    if(size == 0) {
        return 0;
    }

    file_directory_status = SET;
    fifo_location_status = SET;
    log_location_status = SET;
    return 1;
}

int writeLocations(const char* fileLocationType, const char* location) {

    FILE *fileLocations = fopen("fileLocations.txt", "a");

    if(fileLocations == NULL) {
        perror("Unable to obtain locations, ensure that fileLocations.txt has not been moved");
        return -1;
    }

    const size_t length = strlen(fileLocationType) + strlen(location) + 4;

    char* entry = malloc(length);
    if(entry == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }

    sprintf(entry, "%s = %s", fileLocationType, location);

    printf("Results = %s", entry);

    if(fwrite(entry, sizeof(char), strlen(entry), fileLocations) <= 0) {
        perror("Unable to write to the file");
        free(entry);
        return -1;
    }

    free(entry);
    return 0;
}

static int setLocation(enum FILE_LOCATION_STATUS* status, const char* location, const char* logMessage, const char* errorMessage, int type) {
    if (*status == READY_TO_BE_SET) {
        *status = SET;
        writeToLog(logMessage);
        switch (type) {
            case 1: writeLocations("fileDirectory", location);
            case 2: writeLocations("logLocation", location);
            case 3: writeLocations("fifoPath", location);
            default: perror("No type specified");
        }
        return 0;
    } else {
        perror(errorMessage);
        return -1;
    }
}

int setfileDirectory(const char* location) {
    return setLocation(&file_directory_status, location, "File Directory location Set", "Error: File Directory Location has been set!", 1);
}

int setLogLocation(const char* location) {
    return setLocation(&log_location_status, location, "Log location Set", "Error: Log Location has been set!", 2);
}

int setFIFOLocation(const char* location) {
    return setLocation(&fifo_location_status, location, "FIFO Location Set", "Error: FIFO Location has been set!", 3);
}

const char* getfileDirectory() {
    return fileDirectory;
}

const char* getLogLocation() {
    return logLocation;
}

const char* getFIFOLocation() {
    return fifoPath;
}