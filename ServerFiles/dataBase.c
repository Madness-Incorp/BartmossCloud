//
// Created by Oisin Lynch on 24/10/2024.
//
#include "dataBase.h"
#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "fileHelpers.h"
#include "Encryption/SHA256.h"

int testDBconnection() {

    sqlite3 *database;
    const char rc = sqlite3_open("Accounts.db", &database);

    if(rc) {
        fprintf(stderr, "Cannot open datBase: %s\n", sqlite3_errmsg(database));
        return -1;
    }

    fprintf(stderr, "Opened dataBase successfully\n");
    sqlite3_close(database);
    return 0;
}

char** readInAccount(const int socketID){
  size_t usernameSize = 0, passwordSize = 0;

    // Read username and password sizes
    if (read(socketID, &usernameSize, sizeof(size_t)) <= 0) {
        perror("Error reading username size from client");
        return NULL;
    }
    if (read(socketID, &passwordSize, sizeof(size_t)) <= 0) {
        perror("Error reading password size from client");
        return NULL;
    }

    printf("Username Size: %zu, Password Size: %zu\n", usernameSize, passwordSize);

    // Allocate memory for Username and Password
    char* Username = malloc(usernameSize + 1);
    char* Password = malloc(passwordSize + 1);
    if (Username == NULL || Password == NULL) {
        perror("Memory allocation error");
        free(Username);
        free(Password);
        return NULL;
    }

    // Read Username
    if (read(socketID, Username, usernameSize) <= 0) {
        perror("Error reading Username from client");
        free(Username);
        free(Password);
        return NULL;
    }
    Username[usernameSize] = '\0';  // Null-terminate

    // Read Password
    if (read(socketID, Password, passwordSize) <= 0) {
        perror("Error reading Password from client");
        free(Username);
        free(Password);
        return NULL;
    }
    Password[passwordSize] = '\0';  // Null-terminate

    printf("Received Username: %s\n", Username);
    printf("Received Password: %s\n", Password);

    char** accountData = malloc(2 * sizeof(char*));

    accountData[0] = Username;
    accountData[1] = Password;
    return accountData;
}

int checkAccountDetails(char **passwordAndUsername) {
    printf("Received Username: %s\n", passwordAndUsername[0]);
    printf("Received Password: %s\n", passwordAndUsername[1]);

    // Open SQLite database
    sqlite3 *database;
    const char* databaselocation = getDatabaseLocation();
    int rc = sqlite3_open(databaselocation, &database);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        free(passwordAndUsername[0]);
        free(passwordAndUsername[1]);
        return -1;
    }

    // Prepare SQL statement
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM Account_Data WHERE Username = ? AND Password = ? LIMIT 1;";
    rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        free(passwordAndUsername[0]);
        free(passwordAndUsername[1]);
        return -1;
    }

    const char* hashedPassword = hash(passwordAndUsername[1]);

    // Bind parameters
    sqlite3_bind_text(stmt, 1, passwordAndUsername[0], -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashedPassword, -1, SQLITE_STATIC);

    // Execute statement
    rc = sqlite3_step(stmt);
    const int exists = rc == SQLITE_ROW;

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    free(passwordAndUsername[0]);
    free(passwordAndUsername[1]);

    return exists; // 1 if true, 0 otherwise
}

int checkUsernameUsed(const char* username, sqlite3 *db){
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM Account_Data WHERE Username = ? LIMIT 1;";

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;  // error code
    }

    // Bind the username parameter to the SQL statement
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    // Execute the query
    rc = sqlite3_step(stmt);
    const int exists = rc == SQLITE_ROW;  // If a row is found, username exists

    // Clean up
    sqlite3_finalize(stmt);
    return exists;  // 1 if exists, 0 if not
}

int createNewUser(char**passwordAndUsername) {
    printf("Received Username: %s\n", passwordAndUsername[0]);
    printf("Received Password: %s\n", passwordAndUsername[1]);

    // Open SQLite database
    sqlite3 *database;
    const char* databaselocation = getDatabaseLocation();
    int rc = sqlite3_open(databaselocation, &database);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        free(passwordAndUsername[0]);
        free(passwordAndUsername[1]);
        return -1;
    }

    if(checkUsernameUsed(passwordAndUsername[0], database) == 1){
        sqlite3_close(database);
        free(passwordAndUsername[0]);
        free(passwordAndUsername[1]);
        return 1;
    }

    const char* hashedPassword = hash(passwordAndUsername[1]);

    // Prepare SQL insert statement
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO Account_Data (Created, Last_Accessed, Username, Password, Privilege) VALUES (?, ?, ?, ?, 0);";

    rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return -1;
    }

    const char *currentDate = getCurrentDate();
    if (currentDate == NULL) {
        sqlite3_close(database);
        free(passwordAndUsername[0]);
        free(passwordAndUsername[1]);
        return -1;
    }

    // Bind parameters (dates as placeholders, user input for username and password)
    sqlite3_bind_text(stmt, 1, currentDate, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, currentDate, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, passwordAndUsername[0], -1, SQLITE_STATIC);    
    sqlite3_bind_text(stmt, 4, hashedPassword, -1, SQLITE_STATIC);  

    // Execute SQL statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(database));
        sqlite3_finalize(stmt);
        sqlite3_close(database);
        return -1;
    }

    printf("Records created successfully\n");

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return 0;
}

char* getCurrentDate(){
  const time_t t = time(NULL);
  const struct tm tm = *localtime(&t);

  char* currentDate = malloc(11);
  char* today = malloc(3);
  char* month = malloc(3);

  if(tm.tm_mday < 10){
   if(snprintf(today, 3, "0%d", tm.tm_mday) == 0){
      perror("Error with snprintf");
      free(currentDate);
      free(today);
    }
  }else{
    sprintf(today, "%d", tm.tm_mday);
  }

  if(tm.tm_mon + 1 < 10){
    if(snprintf(month, 3, "0%d", tm.tm_mon+1) == 0){
    perror("Error with snprintf");
    free(currentDate);
    free(today);
    free(month);
    }
  }else{
    sprintf(month, "%d", tm.tm_mon+1);
  }

 if (snprintf(currentDate, 11, "%s-%s-%d", today, month, tm.tm_year + 1900) == 0){
    perror("Error creating the date string");
    free(currentDate);
    free(today);
     free(month);
    return NULL;
 }

  printf("The current date is %s\n", currentDate);

  return currentDate;
}

