//
// Created by oisin on 16/08/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "fileHelpers.h"
#include "fileLocationFunctions.h"
#include "logging.h"


void savefile(const int sockID, const char *filename, const size_t filesize, const bool isServer) {
  char *buffer = malloc(filesize);
  if (buffer == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(-1);
  }

  const ssize_t n = read(sockID, buffer, filesize);
  if (n <= 0) {
    perror("Error receiving the file");
    free(buffer);
    exit(-1);
  }

  char *filepath = NULL;

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

void readIn(const int socketId, char* bufr) {
  ssize_t totalRead = 0;

  while (1) {
    const ssize_t numbRead = read(socketId, bufr + totalRead, 1);
    if (numbRead == -1) {
      fprintf(stderr, "read() error\n");
      close(socketId);
      exit(-1);
    }

    totalRead += numbRead;
    //Read file until break character
    if (bufr[totalRead - 1] == '$') {
      bufr[totalRead - 1] = '\0';
      break;
    }
  }

}

char* readFIFO() {

  const char* fifoPath = getFIFOLocation();
  printf("Start reading\n");
  const int FIFOId = open(fifoPath, O_RDONLY);
  if(FIFOId < 0) {
    perror("Error opening FIFO");
    return NULL;
  }

  size_t fileNameSize = 1, length = 0;
  char* fileName = malloc(fileNameSize * sizeof(char));
  if(fileName == NULL) {
    perror("Malloc failed");
    close(FIFOId);
    return NULL;
  }

  ssize_t readFIFO;
  char byte;

  while ((readFIFO = read(FIFOId, &byte, 1)) > 0) {
    if (length >= fileNameSize - 1) {
      fileNameSize *= 2;
      char *newBuffer = realloc(fileName, fileNameSize * sizeof(char));
      if (newBuffer == NULL) {
        perror("Error with realloc");
        free(fileName);
        close(FIFOId);
        return NULL;
      }
      fileName = newBuffer;
    }
    fileName[length++] = byte;
  }

  fileName[length] = '\0';

  if(readFIFO == -1) {
    perror("Error reading from FIFO");
    return NULL;
  }

  close(FIFOId);

  printf("File Chosen %s\n", fileName);

  return fileName;
}

int dealWithResult(const int socketID) {

  int resultofOperation;
  const char* fifoPath = getFIFOLocation();

  if(read(socketID, &resultofOperation, sizeof(int)) <= 0) {
    writeToLog("ERROR: Reading result from Server");
    perror("ERROR: Reading result from Server");
    return -1;
  }

  const int FIFOId = open(fifoPath, O_WRONLY);
  if(FIFOId < 0) {
    perror("Error opening FIFO");
    return -1;
  }

  if (write(FIFOId, &resultofOperation, sizeof(int)) <= 0) {
    writeToLog("ERROR: Writing result to GUI");
    perror("ERROR: Writing result to GUI");
    return -1;
  }

  close(FIFOId);
  return resultofOperation;
}
