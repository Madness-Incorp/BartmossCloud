//
// Created by Oisin Lynch on 24/10/2024.
//
#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

int testDBconnection();
int createNewUser(char**passwordAndUsername);
int checkUsernameUsed(const char* username, sqlite3 *db);
int checkAccountDetails(char** passwordAndUsername);
char* getCurrentDate();
char** readInAccount(int socketID);

#endif //DATABASE_H
