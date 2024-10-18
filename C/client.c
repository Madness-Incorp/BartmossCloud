//
// Created by Oisin Lynch on 10/08/2

//****NOTE ADD LOG FILE TO KEEP TRACK OPERATIONS DONE ON THE CLIENT SIDE IN A
// LOG FILE***
#include "client.h"
#include "fileHelpers.h"
#include "logging.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVERIP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {

  const char *fileDirectory = "/Users/oisin/Coding/ClientFolder/";
  const char *fifopath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";

  struct Client client = client_constructor(AF_INET, SOCK_STREAM, 4999, 0);

  int cfd = client.socket;
  char bufr[2000];

  // Read welcome messages from the server
  readIn(cfd, bufr);

  fprintf(stdout, "%s", bufr);

  const int upOrDown = getchar();
  while (getchar() != '\n')

    fprintf(stdout, "You entered: %c %lu\n", upOrDown, sizeof(upOrDown));

  // Ensure that the letter sent to the server is a capital
  int upLetter;
  if (upOrDown >= 65 && upOrDown <= 90) {
    upLetter = upOrDown;
  } else {
    upLetter = toupper(upOrDown);
  }

  char *choiceMessage = malloc(sizeof(char) * 14);
  char upString[2];
  upString[0] = (char)upLetter;
  upString[1] = '\0';
  strcat(choiceMessage, upString);
  strcat(choiceMessage, " was chosen\n");
  writeToLog(choiceMessage, 'c');

  ssize_t writeDorU = write(cfd, &upLetter, sizeof(char));
  if (writeDorU <= 0) {
    fprintf(stderr, "Error writing to the server");
    exit(EXIT_FAILURE);
  }

  if (upLetter == 'D') {
    memset(bufr, 0, sizeof(bufr));
    bufr[0] = '\0';
    fprintf(stdout, "Choose a file you would like to Download\n");
    readIn(cfd, bufr);
    fprintf(stdout, "Choices: %s\n", bufr);

    int fd = open(fifopath, O_WRONLY);
    if(fd == -1) {
      perror("Error opening FIFO");
      return -1;
    }

    const ssize_t bytesWritten = write(fd, bufr, strlen(bufr));
    if(bytesWritten == 0) {
      perror("Error writing to FIFO");
      close(fd);
      return -1;
    }

    printf("Files written to FIFO\n");

    close(fd);

    fd = open(fifopath, O_RDONLY);

    size_t bufferSize = 1;
    char *line = malloc(bufferSize * sizeof(char));

    if (line == NULL) {
      perror("malloc failed");
      close(cfd);
      return -1;
    }
    size_t length = 0;
    if (fd == -1) {
      perror("Error opening FIFO");
      return -1;
    }

    ssize_t readuser;
    char byte;

    while ((readuser = read(fd, &byte, 1)) > 0) {
      if (length >= bufferSize - 1) {
        bufferSize *= 2;
        char *newBuffer = realloc(line, bufferSize * sizeof(char));
        if (newBuffer == NULL) {
          perror("Error with realloc");
          free(line);
          close(cfd);
          close(fd);
          return -1;
        }
        line = newBuffer;
      }
      line[length++] = byte;
    }

    line[length] = '\0';

    if (readuser == -1) {
      return -1;
    }

    close(fd);
    // Remove newline character from input
    char* newMessage = malloc((strlen(line) + 2) * sizeof(char));  // +2 to account for '$' and '\0'

    if (newMessage == NULL) {
      perror("malloc failed");
      free(line);
      close(cfd);
      return -1;
    }

    newMessage[0] = '\0';
    strcat(newMessage, line);
    strcat(newMessage, "$");

    printf("FILE NAME: %s\n", newMessage);

    writeToLog("File choice written to server\n", 'c');
    const ssize_t answer = write(cfd, newMessage, strlen(newMessage));
    if (answer <= 0) {
      fprintf(stderr, "Error with writing to server");
      close(cfd);
      exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Choice sent to server\n");

    // Recieve the size of the file
    size_t filesize;
    writeToLog("File size received from the server\n", 'c');
    read(cfd, &filesize, sizeof(filesize));

    fprintf(stdout, "File size: %lu\n", filesize);

    // Receive the file content
    writeToLog("Downloading the file from the server\n", 'c');
    savefile(cfd, line, filesize, false);
  } else if (upLetter == 'U') {
    fprintf(stdout, "Choose a file you would like to upload to the Cloud\n");

    char *list = displayDirectory(fileDirectory);
    fprintf(stdout, "%s\n", list);
    char *duplist = strdup(list);
    char **choice = fileToArray(duplist, strlen(list));

    ssize_t bufferSize = 1;
    char *line = malloc(bufferSize * sizeof(char));

    if (line == NULL) {
      perror("malloc failed");
      close(cfd);
      return -1;
    }
    size_t length = 0;

    const int fd = open(fifopath, O_RDONLY);
    if (fd == -1) {
      perror("Error opening FIFO");
      return -1;
    }

    ssize_t readuser;
    char byte;

    while ((readuser = read(fd, &byte, 1)) > 0) {
      if (length >= bufferSize - 1) {
        bufferSize *= 2;
        char *newBuffer = realloc(line, bufferSize * sizeof(char));
        if (newBuffer == NULL) {
          perror("Error with realloc");
          free(line);
          close(cfd);
          close(fd);
          return -1;
        }
        line = newBuffer;
      }
      line[length++] = byte;
    }

    line[length] = '\0';

    if (readuser == -1) {
      return -1;
    }

    // Remove newline character from input
    printf("FILE NAME: %s\n", line);
    int flag = 0;
    int i = 0;

    while (choice[i] != NULL) {
      if (strcmp(choice[i], line) == 0) {
        flag = 1;
        break;
      }
      i++;
    }

    if (flag == 0) {
      perror("File does not exist");
    }

    char *fullFilename = malloc(strlen(fileDirectory) + strlen(line));
    if (fullFilename == NULL) {
      perror("Memory allocation failed");
      close(cfd);
      exit(EXIT_FAILURE);
    }
    fullFilename[0] = '\0';
    strcat(fullFilename, fileDirectory);
    strcat(fullFilename, line);

    printf("Location %s\n", fullFilename);

    FILE *fp = fopen(fullFilename, "r");
    if (fp == NULL) {
      perror("File not found");
      close(cfd);
      exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    const size_t filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    writeToLog("Sending file size to Server\n", 'c');
    const ssize_t fileSizeWritten = write(cfd, &filesize, sizeof(size_t));
    if (fileSizeWritten <= 0) {
      fprintf(stderr, "Write error for sending file size\n");
      close(cfd);
    } else {
      fprintf(stdout, "File size was sent to the client\n");
    }

    char *fullName =
        malloc(strlen(line) + 2); // +2 for '$' and the null terminator
    if (fullName == NULL) {
      perror("Memory allocation failed");
      close(cfd);
      exit(EXIT_FAILURE);
    }

    fullName[0] = '\0'; // Initialize the buffer as an empty string
    strcat(fullName, line);
    strcat(fullName, "$");

    const size_t length_line = strlen(fullName);
    writeToLog("File name length was sent to the server\n", 'c');
    ssize_t sizeWritten = write(cfd, &length_line, sizeof(size_t));
    if (sizeWritten <= 0) {
      perror("Issue writting to server");
      exit(EXIT_FAILURE);
    }

    writeToLog("File name was sent to the server\n", 'c');
    sizeWritten = write(cfd, fullName, length_line);
    if (sizeWritten <= 0) {
      perror("Issue writting to the server");
      exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Sending file to the Server");
    send_file(fp, cfd, filesize);
    fprintf(stdout, "File sent successfully\n");
    fclose(fp);
  } else {
    perror("Invalid input");
    exit(-1);
  }
  close(cfd);
  return 0;
}
