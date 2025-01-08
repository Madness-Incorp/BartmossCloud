#include <stdio.h>
#include "importantFileLocations.h"
#include "logging.h"
#include "fileLocationFunctions.h"

static enum FILE_LOCATION_STATUS file_directory_status = READY_TO_BE_SET;
static enum FILE_LOCATION_STATUS log_location_status = READY_TO_BE_SET;
static enum FILE_LOCATION_STATUS fifo_location_status = READY_TO_BE_SET;

static int setLocation(const char** variable, enum FILE_LOCATION_STATUS* status, const char* location, const char* logMessage, const char* errorMessage) {
    if (*status == READY_TO_BE_SET) {
        *variable = (char*)location;
        *status = SET;
        writeToLog(logMessage);
        return 0;
    } else {
        perror(errorMessage);
        return -1;
    }
}

int setfileDirectory(const char* location) {
    return setLocation(&fileDirectory, &file_directory_status, location, "File Directory location Set", "Error: File Directory Location has been set!");
}

int setLogLocation(const char* location) {
    return setLocation(&logLocation, &log_location_status, location, "Log location Set", "Error: Log Location has been set!");
}

int setFIFOLocation(const char* location) {
    return setLocation(&fifoPath, &fifo_location_status, location, "FIFO Location Set", "Error: FIFO Location has been set!");
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