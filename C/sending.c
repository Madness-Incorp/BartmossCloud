//
// Created by Oisin Lynch on 20/08/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fileHelpers.h"

void send_file(FILE * fp, int sockID, size_t filesize) {
    char * buffer = malloc(filesize);
    if (buffer == NULL) {
        fprintf(stderr, "Issue with memory allocation\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, filesize, fp);

    if (write(sockID, buffer, filesize) == -1) {
        fprintf(stderr, "Error with sending the file\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    free(buffer);
}
