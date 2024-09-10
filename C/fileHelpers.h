//
// Created by oisin on 16/08/24.
//

#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

void send_file(FILE *fp, int sockID, size_t filesize);
char* displayDirectory(char* direcPath);
char** fileToArray(char* listOfFiles, size_t lofSize);
char **listFiles(char *directoryPath); 
void savefile(int sockID, const char *filename, size_t filesize, bool isServer);
void readIn(int socketId, char* bufr);

#endif //FILEHELPERS_H
