#include <stdio.h>
#include "importantFileLocations.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "logging.h"
#include "fileLocationFunctions.h"

static enum FILE_LOCATION_STATUS file_directory_status = READY_TO_BE_SET;
static enum FILE_LOCATION_STATUS log_location_status = READY_TO_BE_SET;

char *getFileOrFIFOLocation(const int choice) {
    const char *homeDir = getenv("HOME");

    if (homeDir == NULL) {
        perror("Unable to get home Directory");
        return NULL;
    }

    size_t pathLength = 0;

    if (choice == 1) {
        pathLength = strlen(homeDir) + strlen("/BartmossCloud/fileLocations.txt") + 1;
    }
    if (choice == 2) {
        pathLength = strlen(homeDir) + strlen("/BartmossCloud/clientFIFO") + 1;
    }

    if (choice != 1 && choice != 2) {
        return NULL;
    }


    char *filePath = malloc(pathLength);

    if (filePath == NULL) {
        perror("Memroy Allocation Failed");
        return NULL;
    }

    if (choice == 1) {
        snprintf(filePath, pathLength, "%s/BartmossCloud/fileLocations.txt", homeDir);
    }
    if (choice == 2) {
        snprintf(filePath, pathLength, "%s/BartmossCloud/clientFIFO", homeDir);
    }

    printf("File path is %s\n", filePath);

    return filePath;
}

const char *getFileLocationsFileLocation() {
    return getFileOrFIFOLocation(1);
}

const char *getFIFOLocation() {
    return getFileOrFIFOLocation(2);
}

int checkIfLocationsSet() {
    const char *fileLocationsFile = getFileLocationsFileLocation();
    if (fileLocationsFile == NULL) {
        file_directory_status = READY_TO_BE_SET;
        log_location_status = READY_TO_BE_SET;
        free((char *) fileLocationsFile);
        return 0;
    }
    FILE *fileLocations = fopen(fileLocationsFile, "r");

    if (fileLocations == NULL) {
        perror("Unable to obtain locations, ensure that fileLocations.txt has not been moved");
        exit(EXIT_FAILURE);
    }

    fseek(fileLocations, 0, SEEK_END);
    const size_t size = ftell(fileLocations);

    if (size == 0) {
        file_directory_status = READY_TO_BE_SET;
        log_location_status = READY_TO_BE_SET;
        return 0;
    }

    file_directory_status = SET;
    log_location_status = SET;
    return 1;
}

int writeLocations(const char *fileLocationType, const char *location) {
    printf("fileLocationtType: %s\n", fileLocationType);
    printf("Location is: %s\n", location);

    const char *fileLocationsFile = getFileLocationsFileLocation();

    FILE *fileLocations = fopen(fileLocationsFile, "a");

    if (fileLocations == NULL) {
        perror("Unable to obtain locations, ensure that fileLocations.txt has not been moved");
        return -1;
    }

    const size_t length = strlen(fileLocationType) + strlen(location) + 4;

    char *entry = malloc(length);
    if (entry == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }

    sprintf(entry, "%s = %s", fileLocationType, location);

    printf("Results = %s\n", entry);

    if (fwrite(entry, sizeof(char), strlen(entry), fileLocations) <= 0) {
        perror("Unable to write to the file");
        free(entry);
        return -1;
    }

    fclose(fileLocations);
    free(entry);
    return 0;
}

int setLocation(enum FILE_LOCATION_STATUS *status, const char *location, const char *logMessage,
                const char *errorMessage, const int type) {
    if (*status == READY_TO_BE_SET) {
        *status = SET;
        writeToLog(logMessage);
        switch (type) {
            case 1:
                writeLocations("fileDirectory", location);
                break;
            case 2:
                writeLocations("logLocation", location);
                break;
            default: perror("No type specified");
        }
        return 0;
    } else {
        perror(errorMessage);
        return -1;
    }
}

char *trim_whitespace(char *str) {
    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
        return str;

    // Trim trailing space
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    return str;
}

char *getFileDirectoryOrLogLocation(const int choice) {
    const char *fileLocation = getFileLocationsFileLocation();
    FILE *fp = fopen(fileLocation, "r");
    if (fp == NULL) {
        perror("Unable to open file");
        return NULL;
    }

    const int lineNumber = choice - 1;
    int count = 0;

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (count == lineNumber) {
            char *equals_sign = strchr(line, '=');
            if (equals_sign != NULL) {
                const char *value = trim_whitespace(equals_sign + 1);

                char *result = malloc(strlen(value) + 1);
                if (result == NULL) {
                    perror("Memory allocation failed");
                    fclose(fp);
                    return NULL;
                }

                strcpy(result, value);
                fclose(fp);
                return result;
            }
        }
        count++;
    }

    fclose(fp);
    return NULL;
}

const char *getfileDirectory() {
    return getFileDirectoryOrLogLocation(1);
}

const char *getLogLocation() {
    return getFileDirectoryOrLogLocation(2);
}


enum FILE_LOCATION_STATUS *getFileDirectoryStatus() {
    return &file_directory_status;
}

enum FILE_LOCATION_STATUS *getLogLocationStatus() {
    return &log_location_status;
}
