/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Server
*/

#ifndef SER
#define SER

#include "entities/Enemies.hpp"
#include "server/ClientServer.hpp"
#include <vector>
#include <sys/select.h>
#include <cmath>
#include "Game.hpp"


std::vector<std::string> generator(int x, int y);

class Game;
class Server
{
    public:
        Server(int port);
        ~Server();
        void run(void);
        void closeServer(void);
        bool checkIfClientExists(sockaddr_in *so);
        int setFd();
        void manageFs();
        void manageSocket();
        void getCommand(size_t &index);
        void placeBombe(Player *);
        void placeBombeE(Enemies *);

    private:
        std::vector<ClientServer *> clients;
        int socketFd;
        fd_set readfds;
        socklen_t addrlen;
        Game game;
        float timeLaunched;

        Entity *bombeE;
};

#endif
