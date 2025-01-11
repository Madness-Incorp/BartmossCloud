#define BUFSIZE 1024
#include <ctype.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "server.h"
#include "fileHelpers.h"
#include "dataBase.h"
#include "logging.h"

int main() {
  const struct Server server = server_constructor(AF_INET, SOCK_STREAM, 4999, 0);

  getDirectoryLocationFromUser();

  fprintf(stdout, "Listening on (%u)\n", server.address.sin_addr.s_addr);
  getCurrentDate();

  const char * helloMesage = {
    "Hello and welcome to Bartmoss Cloud\n$"
  };
  const char* serverLocation = getServerDirectoryLocation();
  char * list = displayDirectory(serverLocation);
  const char * listChoices = strcat(list, "\n");
  char * duplist = strdup(list);
  char ** choice = fileToArray(duplist, strlen(list));

  for (;;) {
    struct sockaddr_storage claddr;
    socklen_t addrlen = sizeof(struct sockaddr_storage);
    const int acceptId = accept(server.socket, (struct sockaddr * ) & claddr, & addrlen);

    if (acceptId == -1) {
      fprintf(stderr, "accept() error\n");
      close(server.socket);
      close(acceptId);
      exit(EXIT_FAILURE);
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (getnameinfo((struct sockaddr * ) & claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
      fprintf(stdout, "Connection from (%s, %s)\n", host, service);
    } else {
      fprintf(stderr, "Connection from (?UNKNOWN?)\n");
    }

    //Send Hello Message
    ssize_t numWritten = write(acceptId, helloMesage, strlen(helloMesage));
    if (numWritten <= 0) {
      fprintf(stderr, "Write error for the intro message\n");
      close(acceptId);
      continue;
    }

    writeToLog("Intro Message sent to Client");

    const int result = testDBconnection();
    if(result == 0){writeToLog("Connection to DataBase Successful");}
    if(write(acceptId, &result, sizeof(int)) == 0) {
      writeToLog("ERROR: Unable to send database connection result to Clinet");
      perror("Error writing to client");
      close(acceptId);
      continue;
    }
    if(result == -1) {
      writeToLog("ERROR: Connection to Database was unsuccessful");
      perror("Error connecting to Database!!");
      close(acceptId);
      return -1;
    }


    char** passwordAndUsername = readInAccount(acceptId);
    const char operation = readInOperationChoice(acceptId);

    printf("This is the operation %c", operation);

    //C means that a new user wants to be created
    if(operation == 'C'){
      const int resultOfAccountCreation = createNewUser(passwordAndUsername);
      int resultToSend = 0;
      if(resultOfAccountCreation == -1) {
        fprintf(stderr, "Unable to create new account");
        resultToSend = -1;
        if(write(acceptId, &resultToSend, sizeof(int) <= 0)) {
          perror("Error writng result to Client");
        }
        close(acceptId);
        continue;
      }
      if(resultOfAccountCreation == 1){
        resultToSend = 5;
        fprintf(stderr, "Username already exists");
        if(write(acceptId, &resultToSend, sizeof(int) <= 0)) {
          perror("Error writng result to Client");
        }
        close(acceptId);
        continue;
      }
      resultToSend = 1;
      if(write(acceptId, &resultToSend, sizeof(int) <= 0)) {
        perror("Error writng result to Client");
        close(acceptId);
        continue;
      }
    }
    //V means to check account data to see if there is an account with that username and password
    else if (operation == 'V'){
      const int resultOfDetailsCheck = checkAccountDetails(passwordAndUsername);
      if(resultOfDetailsCheck == 0) {
        fprintf(stderr, "Incorrect details");
        if(write(acceptId, &resultOfDetailsCheck, sizeof(int) <= 0)) {
          perror("Error writng result to Client");
        }
        close(acceptId);
        continue;
      }
      if(resultOfDetailsCheck == 1){
        fprintf(stdout, "Correct Account Details\n");
        const char* defaultMessage = " logged into the server";
        char* userLoginMessage = malloc(strlen(passwordAndUsername[0]) + strlen(defaultMessage) + 1);
        strcat(userLoginMessage, passwordAndUsername[0]);
        strcat(userLoginMessage, defaultMessage);
        writeToLog(userLoginMessage);
        if(write(acceptId, &resultOfDetailsCheck, sizeof(int)) <= 0) {
          perror("Error writng result to Client");
          close(acceptId);
          continue;
        }
        free(userLoginMessage);
      }
    }else {
      writeToLog("ERROR: Reading in operation choice");
      close(acceptId);
      continue;
    }

    //Send Server Files
    printf("%s\n", "Sending Server Files");
    strcat(list, "$");
    numWritten = write(acceptId, listChoices, strlen(listChoices));
    if (numWritten <= 0) {
      fprintf(stderr, "Write error for writing choice to Client\n");
      close(acceptId);
    } else {
      fprintf(stdout, "List choices sent to client\n");
    }

    /*
     *Read in from which actor file got chose
     *Either a file from the Server, signaling a download
     *Or a file locally from the Client, signaling an upload
     */
    char choiceOfService;
    read(acceptId, &choiceOfService, sizeof(char));

    fprintf(stderr, "Recieved: %c\n", choiceOfService);

    if (choiceOfService == 'D') {

      char bufr[1024];

      readIn(acceptId, bufr);

      int flag = 0;
      int i = 0;
      while (choice[i] != NULL) {
        if (strcmp(choice[i], bufr) == 0) {
          flag = 1;
          break;
        }
        i++;
      }
      choice = NULL;

      if (flag == 0) {
        perror("File does not exist");
        continue;
      }

      const char * fileDirectory = serverLocation;
      const size_t pathLen = strlen(fileDirectory);
      const size_t fileLen = strlen(bufr);
      char * filename = malloc(pathLen + fileLen + 1); // +1 for the null terminator
      if (filename == NULL) {
        perror("Memory allocation failed");
        close(acceptId);
        exit(EXIT_FAILURE);
      }

      strcpy(filename, fileDirectory); // Copy the directory path
      strcat(filename, bufr);

      printf("Location %s\n", filename);

      FILE * fp = fopen(filename, "r");
      if (fp == NULL) {
        perror("File not found");
        close(acceptId);
        exit(EXIT_FAILURE);
      }

      fseek(fp, 0L, SEEK_END);
      long filesize = ftell(fp);
      fseek(fp, 0L, SEEK_SET);

      fprintf(stdout, "Sending file size to client\n");
      const int fileSizeWritten = write(acceptId, & filesize, sizeof(filesize));
      if (fileSizeWritten <= 0) {
        fprintf(stderr, "Write error for sending file size\n");
        close(acceptId);
      } else {
        fprintf(stdout, "File size was sent to the client\n");
      }

      fprintf(stdout, "Sending file to the client");
      send_file(fp, acceptId, filesize);
      fprintf(stdout, "File sent successfully\n");
      fclose(fp);
    } else {

      size_t filesize = 0;
      read(acceptId, &filesize, sizeof(size_t));
      fprintf(stdout, "File size: %zu\n", filesize);

      printf("Start reading\n");

      ssize_t filenameLength = 0;
      read(acceptId, &filenameLength, sizeof(size_t));
      printf("Length %zu\n", filenameLength);
      char* filename = malloc(filenameLength + sizeof(char));  // Allocate enough memory for the filename
      if (filename == NULL) {
        perror("Memory allocation failed");
        close(acceptId);
        exit(EXIT_FAILURE);
      }
      filename[0] = '\0';  // Initialize the first character

      size_t totalRead = 0;

      while (totalRead < filenameLength + 1) {  // Ensure you don’t read more than the allocated space
        const size_t numbRead = read(acceptId, filename + totalRead, 1);  // Read 1 byte at a time and store it at the correct position
        if (numbRead == -1) {
          fprintf(stderr, "read() error\n");
          close(acceptId);
          free(filename);
          exit(-1);
        }

        if (numbRead == 0) {
          fprintf(stderr, "Connection closed prematurely\n");
          close(acceptId);
          free(filename);
          exit(-1);
        }

        // Check if the break character ('$') is found
        if (filename[totalRead] == '$') {
          filename[totalRead] = '\0';  // Replace '$' with null terminator
          break;
        }

        totalRead += numbRead;  // Increase totalRead by the number of bytes read
      }

      printf("Filename: %s\n", filename);

      //Receive the file content
      savefile(acceptId, filename, filesize, true);
      free(filename);
    }
    close(acceptId);
  }
}

