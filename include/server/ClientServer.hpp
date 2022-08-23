/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Client
*/

#ifndef CLIENTSERVER_HPP_
#define CLIENTSERVER_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

class ClientServer {
    public:
        ClientServer(struct sockaddr_in s, int fd);
        ~ClientServer();
        struct sockaddr_in sockaddr;
        std::string command;
        size_t indexPlayer = -1;
        size_t indexPlayer2 = -1;

        int getFd() { return this->fd;}
        bool getReady() const {return this->ready;}
        void setReady(bool r) {this->ready = r;}
        float delta;
        float time;
    private:
        int fd;
        bool ready;

};

#endif /* !CLIENT_HPP_ */
