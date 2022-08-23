/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include "fcntl.h"
#include <sys/select.h>

class Client {
    public:
        Client(int port = 8082);
        ~Client();
        char *sendAndReceive(const char *);
        void doConnect(const char *ip = "127.0.0.1", int nbClient = 1);
        void doDisconnect(void);

        bool isConnected() const { return this->connected;}
        int sock;
        struct sockaddr_in servaddr;
        int nbPlayers;

    protected:
    private:
        bool connected = false;
        int port;
};

#endif /* !CLIENT_HPP_ */
