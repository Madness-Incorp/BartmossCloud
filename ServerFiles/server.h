//
// Created by Oisin Lynch on 10/08/2024.
//

#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

struct Server {
    int domain;
    int service;
    int port;
    int protocol;
    int socket;

    struct sockaddr_in address;
};

struct Server server_constructor(int domain, int service, int port, int protocol);
#endif //SERVER_H
