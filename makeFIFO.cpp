//
// Created by Oisin Lynch on 10/01/2025.
//

#include "makeFIFO.h"

#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>

extern "C"{
    const char* getFIFOLocation();
}

int makeFIFO() {

    const char* fifoPath = getFIFOLocation();

    if(mkfifo(fifoPath, 0666) == -1) {
        perror("mkfifo failed");
        exit(EXIT_FAILURE);
    }

    printf("%s %s\n", "FIFO created at: ", fifoPath);

    return 0;

}
