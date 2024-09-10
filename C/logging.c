//
// Created by Oisin Lynch on 04/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logging.h"


void writeToLog(const char* action, const char actor) {
    FILE *fp;

    const char* currentTime = returnTime();

    fprintf(stdout, "%s\n", action);

    if(actor == 'c') {
        fp = fopen("/Users/oisin/Coding/ClientFolder/Client_Log.txt", "a");
    }else if (actor == 's') {
        fp = fopen("/Users/oisin/Coding/ServerFolder/Server_Log.txt", "a");
    }else {
        perror("Could not find which actor needs to be logged");
        free((void*)currentTime);
        return;
    }

    if(fp == NULL) {
        perror("Failed to open the file");
        free((void*)currentTime);
        return;
    }

    char* entry = malloc(strlen(action) + strlen(currentTime) + 1);
    if(entry == NULL) {
        perror("Failed to allocate memory for the log entry");
        fclose(fp);
        free((void*)currentTime);
        return;
    }

    entry = strcat(entry, currentTime);
    entry = strcat(entry, action);

    const int sizeWritten = fprintf(fp, "%s", entry);
    if(sizeWritten <= 0) {
        perror("Error writing to file");
        free((void*)currentTime);
        return;
    }

    free(entry);
    free((void*)currentTime);
    fclose(fp);
}


const char* returnTime(void) {
    const time_t t = time(NULL);
    const struct tm date = *localtime(&t);

    char *curTime = malloc(31);
    if(curTime == NULL) {
        perror("Failed to allocate memory for the date string");
        exit(EXIT_FAILURE);
    }

    sprintf(curTime, "%d - %d - %d %02d:%02d: ", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min);

    return curTime;
}


