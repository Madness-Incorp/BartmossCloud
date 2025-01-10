//
// Created by Oisin Lynch on 07/01/2025.
//

#ifndef IMPORTFILELOCATIONS_H
#define IMPORTFILELOCATIONS_H

enum FILE_LOCATION_STATUS {
    READY_TO_BE_SET,
    SET
};

extern char *fileDirectory;
extern char *logLocation;
extern char* fifoPath;


#endif //IMPORTFILELOCATIONS_H
