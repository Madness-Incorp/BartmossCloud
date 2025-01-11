//
// Created by Oisin Lynch on 10/08/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include <arpa/inet.h>


#define SERVERIP "127.0.0.1"
#define BACKLOG 5

struct Server server_constructor(const int domain,const int service, int port, const int protocol) {

    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = inet_addr(SERVERIP);

    server.socket = socket(domain, service, protocol);

    if(server.socket == -1) {
        fprintf(stderr, "socket() error\n");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    if(setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        fprintf(stderr, "setsockopt() error\n");
        exit(-1);
    }

    if(bind(server.socket, (struct sockaddr *)&server.address, sizeof(struct sockaddr)) == -1) {
        fprintf(stderr, "bind() error\n");
        exit(EXIT_FAILURE);
    }

    if(listen(server.socket, BACKLOG) == -1) {
        fprintf(stderr, "listen() error");
    }

    struct sockaddr_in local_addr;
    socklen_t addrlen = sizeof(local_addr);

    if (getsockname(server.socket, (struct sockaddr *)&local_addr, &addrlen) == -1) {
        perror("getsockname");
        exit(EXIT_FAILURE);
    }

    // Convert the binary address to a string
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(local_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    printf("Server bound to IP: %s\n", ip_str);

    return server;
}
