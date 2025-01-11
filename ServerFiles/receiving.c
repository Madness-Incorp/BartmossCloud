//
// Created by oisin on 16/08/24.
//

#include "fileHelpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void savefile(const int sockID, const char *filename, const size_t filesize, const bool isServer) {
  char *buffer = malloc(filesize);
  if (buffer == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(-1);
  }

  const int n = read(sockID, buffer, filesize);
  if (n <= 0) {
    perror("Error receiving the file");
    free(buffer);
    exit(-1);
  }

  char *filepath;

  // Creates the path so the file will be installed in the Downloads folder
  if (isServer == false) {
    const size_t path_len = snprintf(NULL, 0, "%s/Downloads/", getenv("HOME")) + 1;
    filepath = malloc(path_len + strlen(filename) + 1); // +1 for the null terminator
    if (filepath == NULL) {
      perror("Memory allocation failed");
      free(buffer);
      exit(EXIT_FAILURE);
    }
    snprintf(filepath, path_len, "%s/Downloads/", getenv("HOME"));
    strcat(filepath, filename);
  } else {
    // Server side path creation
    const char *filepath2 = getServerDirectoryLocation();
    filepath = malloc(strlen(filepath2) + strlen(filename) +
                      1); // +1 for the null terminator
    if (filepath == NULL) {
      perror("Memory allocation failed");
      free(buffer);
      exit(EXIT_FAILURE);
    }
    filepath[0] = '\0'; // Initialize the buffer with an empty string
    strcat(filepath, filepath2);
    strcat(filepath, filename);
  }

  FILE *fp = fopen(filepath, "wb");
  if (fp == NULL) {
    perror("Error in opening file.");
    free(buffer);
    free(filepath);
    exit(1);
  }

  fwrite(buffer, 1, n, fp);
  printf("File received successfully. Saved to %s\n", filepath);

  fclose(fp);
  free(buffer);
  free(filepath);
}

void readIn(const int socketId, char *bufr) {
  printf("Start reading\n");
  int totalRead = 0;

  while (1) {
    const int numbRead = read(socketId, bufr + totalRead, 1);
    if (numbRead == -1) {
      fprintf(stderr, "read() error\n");
      close(socketId);
      exit(-1);
    }

    totalRead += numbRead;
    // Read file until break character
    if (bufr[totalRead - 1] == '$') {
      bufr[totalRead - 1] = '\0';
      break;
    }
  }
}

char readInOperationChoice(const int socketID){

  char operation = ' ';

  if(read(socketID, &operation, sizeof(char)) == 0){
    perror("Error reading operation choice");
  }

  return operation;
}
