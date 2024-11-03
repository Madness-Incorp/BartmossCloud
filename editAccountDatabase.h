//
// Created by Oisin Lynch on 01/11/2024.
//

#ifndef EDITACCOUNTDATABASE_H
#define EDITACCOUNTDATABASE_H
#include <string>


class editAccountDatabase {

public:
    static int testConnection(bool connectionNeeded);
    static int addNewAccount( std::string username, std::string password);
};



#endif //EDITACCOUNTDATABASE_H
