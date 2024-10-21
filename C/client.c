//
// Created by Oisin Lynch on 10/08/2

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

const char *fifopath = "/Users/oisin/CLionProjects/pipingTest/my_fifo";
const char *fileDirectory = "/Users/oisin/Coding/ClientFolder/";

int main() {

  const struct Client client = client_constructor(AF_INET, SOCK_STREAM, 4999, 0);

  const int cfd = client.socket;
  char bufr[2000];

  // Read welcome messages from the server
  readIn(cfd, bufr);
  fprintf(stdout, "%s", bufr);

  //Read in the Server Files
  printf("Reading in Server Files");
  memset(bufr, 0, sizeof(bufr));
  bufr[0] = '\0';
  fprintf(stdout, "Choose a file you would like to Download\n");
  readIn(cfd, bufr);
  fprintf(stdout, "Choices: %s\n", bufr);

  //Write the server files to the GUI
  if(writeFIFO(bufr) == -1) {
    perror("Error writing to FIFO");
    close(cfd);
    return -1;
  }

  /*
   *Read in from which actor file got chose
   *Either a file from the Server, signaling a download
   *Or a file locally from the Client, signaling an upload
   */
  char modeChosen;
  const int fifoID = open(fifopath, O_RDONLY);
  read(fifoID, &modeChosen, sizeof(char));
  close(fifoID);
  char * fileChosen = readFIFO();
  printf("The file name is: %s\n", fileChosen);

  fprintf(stdout, "You entered: %c %lu\n", modeChosen, sizeof(char));


  char *choiceMessage = malloc(sizeof(char) * 14);
  char upString[2];
  upString[0] = modeChosen;
  upString[1] = '\0';
  strcat(choiceMessage, upString);
  strcat(choiceMessage, " was chosen\n");
  writeToLog(choiceMessage, 'c');

  //Write the mode to the server
  const ssize_t writeModeChoice = write(cfd, &modeChosen, sizeof(char));
  if (writeModeChoice <= 0) {
    fprintf(stderr, "Error writing to the server");
    exit(EXIT_FAILURE);
  }

  if (modeChosen == 'D') {

    // Add dollar to the end of the string
    char* newMessage = convertToDollarString(fileChosen);
    if (newMessage == NULL) {
      perror("Error converting to DollarString");
      close(cfd);
      return -1;
    }

    printf("FILE NAME: %s\n", newMessage);

    writeToLog("File choice written to server\n", 'c');
    sendMessage(cfd, newMessage);
    fprintf(stdout, "Choice sent to server\n");

    // Recieve the size of the file
    size_t filesize;
    writeToLog("File size received from the server\n", 'c');
    read(cfd, &filesize, sizeof(filesize));
    fprintf(stdout, "File size: %lu\n", filesize);

    // Receive the file content
    writeToLog("Downloading the file from the server\n", 'c');
    savefile(cfd, fileChosen, filesize, false);
  } else if (modeChosen == 'U') {

    //Read in files from the specified folder and send each file to the GUI via FIFO
    char *list = displayDirectory(fileDirectory);
    fprintf(stdout, "%s\n", list);
    char *duplist = strdup(list);
    char **choice = fileToArray(duplist);

    if(fileChosen == NULL) {
      perror("Error reading from FIFO");
      close(cfd);
      free(choiceMessage);
      return -1;
    }

    // Remove newline character from input
    printf("FILE NAME: %s\n", fileChosen);
    int flag = 0;
    int i = 0;

    while (choice[i] != NULL) {
      if (strcmp(choice[i], fileChosen) == 0) {
        flag = 1;
        break;
      }
      i++;
    }

    if (flag == 0) {
      perror("File does not exist");
    }

    char *fullFilename = malloc(strlen(fileDirectory) + strlen(fileChosen));
    if (fullFilename == NULL) {
      perror("Memory allocation failed");
      close(cfd);
      exit(EXIT_FAILURE);
    }
    fullFilename[0] = '\0';
    strcat(fullFilename, fileDirectory);
    strcat(fullFilename, fileChosen);

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

    char *fullName = malloc(strlen(fileChosen) + 2);
    if (fullName == NULL) {
      perror("Memory allocation failed");
      close(cfd);
      exit(EXIT_FAILURE);
    }

    fullName[0] = '\0'; // Initialize the buffer as an empty string
    strcat(fullName, fileChosen);
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
