//
// Created by oisin on 16/08/24.
//

#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <stdbool.h>
#include <stdio.h>

extern char* directoryLocation;
extern char* logLocation;
extern char* databaseLocations;

void send_file(FILE *fp, int sockID, size_t filesize);
char* getServerDirectoryLocation();
int getDirectoryLocationFromUser();
char *displayDirectory(const char *directoryPath);
char **fileToArray(char *listOfFiles, size_t lofSize);
char **listFiles(char *directoryPath);
void savefile(int sockID, const char *filename, size_t filesize, bool isServer);
void readIn(int socketId, char *bufr);
char readInOperationChoice(int socketID);
char *getLogLocation();
char* getDatabaseLocation();

#endif // FILEHELPERS_H
