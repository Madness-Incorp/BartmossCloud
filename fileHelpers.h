//
// Created by oisin on 16/08/24.
//

#ifndef FILEHELPERS_H
#define FILEHELPERS_H

#include <stdio.h>
#include <stdbool.h>

void send_file(FILE *fp, int sockID, size_t filesize);
char* displayDirectory(const char* directoryPath);
char** fileToArray(char* listOfFiles);
void savefile(int sockID, const char *filename, size_t filesize, bool isServer);
void readIn(int socketId, char* bufr);
char* readFIFO();
void sendMessage(int socketID, const char* message);
char* convertToDollarString(char* string);
int writeFIFO(const char* string);
char readServerOperation();
int sendOperationChoice(int socketID, char operation);
int dealWithResult(int socketID);

#endif //FILEHELPERS_H
