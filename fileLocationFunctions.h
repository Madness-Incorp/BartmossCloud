//
// Created by Oisin Lynch on 08/01/2025.
//

#ifndef FILELOCATIONFUNCTIONS_H
#define FILELOCATIONFUNCTIONS_H

int setfileDirectory(const char* location);
int setLogLocation(const char* location);
int setFIFOLocation(const char* location);
const char* getfileDirectory();
const char* getLogLocation();
const char* getFIFOLocation();
int checkIfLocationsSet();

#endif //FILELOCATIONFUNCTIONS_H
