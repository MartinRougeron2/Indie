/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Client
*/

#include "Client.hpp"
#include "string.h"
#include "server/Server.hpp"
#include <ctime>

Client::Client(int port)
{
    this->connected = false;
    this->port = port;
}

Client::~Client()
{
}

void Client::doConnect(const char *ip, int nbClient)
{
    std::time_t timeStart = std::time(NULL);

    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
    }
    this->servaddr.sin_family = AF_INET;
    this->servaddr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, ip, &this->servaddr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
    }
    while (!this->connected) {
        if (std::time(NULL) - timeStart > 1)
            break;
        if (connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            this->connected = false;
        } else {
            this->connected = true;
            for (int i = 0; i < nbClient; i++)
                free(this->sendAndReceive("connect\n"));
        }
    }
}

void Client::doDisconnect(void)
{
    if (this->connected) {
        free(sendAndReceive("disconnect\n"));
        this->connected = false;
        close(this->sock);
    }
}


char *Client::sendAndReceive(const char *msg)
{
    char buffer[50000] = {0};
    int n;

    send(this->sock, msg, strlen(msg), 0); // ! ERR => SIGPIPE
    n = recv(this->sock, buffer, 49999, 0);
    buffer[n] = 0;
    return strdup(buffer); // ! ERR
}
