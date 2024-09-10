//
// Created by Oisin Lynch on 20/08/2024.
//

#ifndef CLIENT_H
#define CLIENT_H


#include <netinet/in.h>

struct Client {
    int domain;
    int service;
    int port;
    int protocol;
    int socket;

    struct sockaddr_in address;
};

struct Client client_constructor(int domain, int service, int port, int protocol);

#endif //CLIENT_H
