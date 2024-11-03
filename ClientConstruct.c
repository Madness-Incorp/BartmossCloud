//
// Created by Oisin Lynch on 20/08/2024.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"
#include <arpa/inet.h>
#define SERVERIP "127.0.0.1"

struct Client client_constructor(const int domain, const int service, int port, const int protocol){

    struct Client client;

    client.domain = domain;
    client.service = service;
    client.port = port;
    client.protocol = protocol;

    client.address.sin_family = domain;
    client.address.sin_addr.s_addr = inet_addr(SERVERIP);
    client.address.sin_port = htons(port);

    client.socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client.socket == -1) {
        fprintf(stderr, "socket() error.\n");
        exit(-1);
    }

    fprintf(stdout, "Waiting for connection\n");

    if (connect(client.socket, (struct sockaddr * ) & client.address, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "connect() error, errno %d\n", errno);
        exit(-1);
    }

    fprintf(stdout, "Connection successful\n");

   return client;
}
