/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Client
*/

#include "server/ClientServer.hpp"

ClientServer::ClientServer(struct sockaddr_in s, int fd)
{
    this->ready = false;
    this->sockaddr = s;
    this->fd = fd;
}

ClientServer::~ClientServer()
{
}
