//
// Created by Oisin Lynch on 07/01/2025.
//

#ifndef IMPORTFILELOCATIONS_H
#define IMPORTFILELOCATIONS_H

enum FILE_LOCATION_STATUS {
    READY_TO_BE_SET,
    SET
};

extern char *fileDirectory = "/Users/oisin/Coding/ClientFolder/";
extern char *logLocation = "/Users/oisin/Coding/ClientFolder/Client_Log.txt";
extern char* fifoPath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";


#endif //IMPORTFILELOCATIONS_H
