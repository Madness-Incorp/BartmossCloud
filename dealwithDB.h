//
// Created by Oisin Lynch on 02/11/2024.
//

#ifndef DEALWITHDB_H
#define DEALWITHDB_H

char testConnection(int socketID);
int sendUsernamePasswordData(int socketID, char mode);
int sendAccountData(int socketID);

#endif //DEALWITHDB_H
