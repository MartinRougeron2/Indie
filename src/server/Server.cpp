/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Server
*/

#include "server/Server.hpp"
#include "entities/Enemies.hpp"
#include "entities/bonus/Capacity.hpp"
#include "unistd.h"
#include "Option.hpp"
#include <algorithm>
#include <iostream>

Vector3 moveWithFacing(Vector3 pos, Cardinal facing)
{
    switch (facing)
    {
        case NORTH:
            pos.z += 1;
            break;
        case SOUTH:
            pos.z -= 1;
            break;
        case WEST:
            pos.x -= 1;
            break;
        default:
            pos.x += 1;
    }
    return pos;
}

Server::Server(int port)
{
    #ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
    this->timeLaunched = GetTime();
    int opt = 1;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    this->addrlen = sizeof(address);
    if ((this->socketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
    }
    if (setsockopt(this->socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
    }
    if (bind(this->socketFd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
    }
    if (listen(this->socketFd, 3) < 0) {
        perror("listen");
    }
    this->game.server = this;
}

Server::~Server()
{
}

void Server::closeServer(void)
{
    shutdown(this->socketFd, SHUT_RDWR);
    close(this->socketFd);
    delete this->bombeE;
    delete game.playerE;
}

int Server::setFd()
{
    int maxSd = this->socketFd;

    if (this->clients.empty())
        return maxSd;
    for (size_t i = 0; i != this->clients.size(); i++) {
        FD_SET(this->clients.at(i)->getFd(), &this->readfds);
        if (this->clients.at(i)->getFd() > maxSd)
            maxSd = this->clients.at(i)->getFd();
    }
    return maxSd;
}

void Server::manageFs()
{
    int maxSd;

    FD_ZERO(&this->readfds);
    FD_SET(this->socketFd, &this->readfds);
    maxSd = this->setFd();
    if (select(maxSd + 1, &this->readfds, NULL, NULL, NULL) < 0
    && errno != EINTR)
        printf("select error");
}

bool operator==(Vector3 a, Vector3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

void Server::manageSocket()
{
    if (FD_ISSET(this->socketFd, &this->readfds)) {
        struct sockaddr_in data;
        int connection = accept(this->socketFd, (struct sockaddr *) &data, &this->addrlen);
        if (connection < 0) {
            perror("accept");
            exit(84);
        }
        this->clients.push_back(new ClientServer(data, connection));
    }
}

void Server::placeBombe(Player *player)
{
    if (player->nbBomb <= 0)
        return;
    Vector3 pos = player->getPosition();
    pos.x = std::roundf(pos.x);
    pos.y = std::roundf(pos.y);
    pos.z = std::roundf(pos.z);
    pos = moveWithFacing(pos, player->getFacing());
    for (auto &e : game.getEntities()) {
        if (e->getPosition() == pos) {
            pos = player->getPosition();
            pos.x = std::roundf(pos.x);
            pos.y = std::roundf(pos.y);
            pos.z = std::roundf(pos.z);
            break;
        }
    }
    game.addEntity(new Bombe(*bombeE, pos, player->getExplodedTime(), player->getPower()));
    player->nbBomb -= 1;
    if (player->placeAt < 0)
        player->placeAt = GetTime();
}

void Server::placeBombeE(Enemies *player)
{
    Vector3 pos = player->getPosition();

    pos.x = std::roundf(pos.x);
    pos.y = std::roundf(pos.y);
    pos.z = std::roundf(pos.z);
    pos = moveWithFacing(pos, player->getFacing());

    for (auto &e : game.getEntities()) {
        if (e->getPosition() == pos) {
            pos = player->getPosition();
            pos.x = std::roundf(pos.x);
            pos.y = std::roundf(pos.y);
            pos.z = std::roundf(pos.z);
            break;
        }
    }
    game.addEntity(new Bombe(*bombeE, pos, 1.0f, player->getPower()));
    player->nbBomb -= 1;
    if (player->placeAt < 0)
        player->placeAt = GetTime();
}

std::string getPlayerStat(Player *p)
{
    std::string tmp;

    tmp = (p->playing) ? "1" : "0";
    tmp += std::to_string(p->getPower());
    tmp += p->getGhost() ? "1" : "0";
    tmp += std::to_string(p->nbBomb);
    tmp += std::to_string(p->getCapacity());
    tmp += std::to_string(static_cast<int>(p->getSpeed() * 100));
    return tmp;
}

void Server::getCommand(size_t &index)
{
    char buffer[4096] = {0};
    int valread;
    std::string tmp = "";
    ClientServer *client = this->clients.at(index);
    size_t newIndex;

    valread = read(client->getFd(), buffer, 4096);
    client->delta = (GetTime() - game.time) * 60.0f;
    client->time = GetTime();
    if (client->command.empty()) {
        client->command = std::string(buffer);
    } else {
        client->command += buffer;
    }
    if (client->command.back() == '\n') {
        if (client->command == "connect\n") {
            newIndex = game.addPlayer(new Player(*game.playerE));
            send(client->getFd(), "connected", 10, 0);
            if (client->indexPlayer != -1)
                client->indexPlayer2 = newIndex;
            else
                client->indexPlayer = newIndex;

        } else if (client->command == "disconnect\n") {
            send(client->getFd(), "disconnected", 13, 0);
            this->clients.erase(this->clients.begin() + index);
            this->game.removePlayer(client->indexPlayer);
            index--;
            return;

        } else if (client->command == "getLevel\n") {
            if (this->game.state == SOLOMODE)
                send(client->getFd(), std::to_string(this->game.level).c_str(), 2, 0);
            else
                send(client->getFd(), "nothing", 8, 0);
        } else if (client->command == "solo\n") {
            game.addEnemie(1);
            game.state = SOLOMODE;

        } else if (client->command == "multi\n") {
            game.state = MULTIMODE;

        } else if (client->command == "getStat1\n") {
            tmp += getPlayerStat(this->game.getPlayer(client->indexPlayer));
            send(client->getFd(), tmp.c_str(), tmp.length(), 0);

        } else if (client->command == "getStat2\n") {
            tmp += getPlayerStat(this->game.getPlayer(client->indexPlayer2));
            send(client->getFd(), tmp.c_str(), tmp.length(), 0);

        } else if (client->command == "getTime\n") {
            tmp = this->game.getTimeGame();
            send(client->getFd(), tmp.c_str(), tmp.length(), 0);


        } else if (client->command == "multiLaunch\n") {
            if (game.state == MULTIMODE)
                send(client->getFd(), "LAUNCH", 7, 0);
            else
                send(client->getFd(), "NOTLAUNCH", 10, 0);


        } else if (this->game.state == SOLOMODE && client->command == "endGame\n") {
            if (game.level == 6)
                send(client->getFd(), "1", 2, 0);
            else if (game.arePlayersAllDead()) {
                for (auto &client : this->clients)
                    send(client->getFd(), "0", 2, 0);
            } else
                send(client->getFd(), "nothing", 8, 0);

        } else if (this->game.state == MULTIMODE && client->command == "endGame\n") {
            int playerIndex = game.areOnePlayerLeft();
            if (playerIndex != -1) {
                for (auto &c : clients) {
                    if (c->indexPlayer == playerIndex)
                        send(c->getFd(), "1", 2, 0);
                    else
                        send(c->getFd(), "0", 2, 0);
                }
            } else
                send(client->getFd(), "nothing", 8, 0);

        } else if (client->command == "clientReady\n") {
            client->setReady(true);
            send(client->getFd(), "nothing", 10, 0);

        } else if (client->command == "clientNotReady\n") {
            client->setReady(false);
            send(client->getFd(), "nothing", 10, 0);

        } else if (client->command == "nbConnected\n") {
            send(client->getFd(), std::to_string(this->clients.size()).c_str(), 10, 0);

        } else if (client->command == "nbReady\n") {
            size_t nbReady = 0;
            for (auto &i: this->clients)
                if (i->getReady())
                    nbReady++;
            std::string nbReadyStr = std::to_string(nbReady);
            send(client->getFd(), nbReadyStr.c_str(), nbReadyStr.length(), 0);

        } else if (client->indexPlayer != -1 && game.state != NOTLAUNCH){
            Player *playerOne = game.getPlayer(client->indexPlayer);
            if (client->command.find(UP) != std::string::npos && playerOne->playing)
                game.movePlayerAt({playerOne->getSpeed() * game.delta, 0, 0}, playerOne);
            if (client->command.find(LEFT) != std::string::npos && playerOne->playing)
                game.movePlayerAt({0, 0, -playerOne->getSpeed() * game.delta}, playerOne);
            if (client->command.find(DOWN) != std::string::npos && playerOne->playing)
                game.movePlayerAt({-playerOne->getSpeed() * game.delta, 0, 0}, playerOne);
            if (client->command.find(RIGHT) != std::string::npos && playerOne->playing)
                game.movePlayerAt({0, 0, playerOne->getSpeed() * game.delta}, playerOne);
            if (client->command.find(BOMB) != std::string::npos && playerOne->playing)
                this->placeBombe(playerOne);
            if (client->indexPlayer2 != -1) {
                Player *playerTwo = game.getPlayer(client->indexPlayer2);
                if (client->command.find(UP2) != std::string::npos && playerTwo->playing)
                    game.movePlayerAt({playerTwo->getSpeed() * game.delta, 0, 0}, playerTwo);
                if (client->command.find(LEFT2) != std::string::npos && playerTwo->playing)
                    game.movePlayerAt({0, 0, -playerTwo->getSpeed() * game.delta}, playerTwo);
                if (client->command.find(DOWN2) != std::string::npos && playerTwo->playing)
                    game.movePlayerAt({-playerTwo->getSpeed() * game.delta, 0, 0}, playerTwo);
                if (client->command.find(RIGHT2) != std::string::npos && playerTwo->playing)
                    game.movePlayerAt({0, 0, playerTwo->getSpeed() * game.delta}, playerTwo);
                if (client->command.find(BOMB2) != std::string::npos && playerTwo->playing)
                    this->placeBombe(playerTwo);
            }
            std::vector<std::string> serialized = SerializeEntity(game.getEntities(), client->indexPlayer);
            std::string bb;
            for (auto a : serialized)
                bb += a;
            if (bb.empty())
                bb = "none";
            send(client->getFd(), bb.c_str(), bb.length(), 0); // ! ERR  Syscall param socketcall.sendto(msg) points to uninitialised byte(s)
        }
        if (client->command == "multi\n" || client->command == "solo\n") {
            game.setTimeGame();
            send(client->getFd(), "nothing", 8, 0);
            this->game.setPlayersPos();
        }
        client->command.clear();
    }
}

bool Server::checkIfClientExists(sockaddr_in *so)
{
    /*for (auto &&e : this->clients)
        if (
            e->sockaddr_in.sin_addr.s_addr == so->sin_addr.s_addr &&
            e->sockaddr_in.sin_port == so->sin_port
        )
            return true;*/
    return false;
}

void Server::run(void)
{
    char buffer[144];
    int max = socketFd;
    fd_set rdfs;
    struct sockaddr_in servaddr, cliaddr;

    auto map = generator(25, 25);
    auto wall = new Entity("assets/model/stone01.obj", {0}, false, true, 0.01f);
    auto ground = new Entity("assets/model/ground.obj", {0}, false, true, 0.01f);
    auto brick = new Entity("assets/model/wallBrick01.obj", {0}, false, true, 0.01f);
    this->bombeE = new Entity("assets/model/water.obj", {0}, false, true, 0.01f);
    auto bombe = new Bombe(*bombeE);

    // game.addEntity(new Capacity(*bombeE, (Vector3) {1, 1, 1}));

    float w = 0.0f, h = 0.0f;
    for (auto a : map) {
        for (auto b : a) {
            switch(b) {
            case '1':
                game.addEntity(new Block(*wall, (Vector3) {w, 1.0f, h}));
                game.addEntity(new Block(*ground, (Vector3) {w, 0.0f, h}));
                break;
            case '0':
                game.addEntity(new Block(*ground, (Vector3) {w, 0.0f, h}));
                break;
            case '2':
                game.addEntity(new Block(*brick, (Vector3) {w, 1.0f, h}, true));
                game.addEntity(new Block(*ground, (Vector3) {w, 0.0f, h}));
                break;
            default:
                game.addEntity(new Block(*ground, (Vector3) {w, 0.0f, h}));
            }
            w += 1.0f;
        }
        w = 0.0f;
        h += 1.0f;
    }
    game.setReady(true);
    game.time = GetTime();
    while (1) {
        this->manageFs();
        this->manageSocket();
        game.processBombe();
        game.processIA();
        game.delta = (GetTime() - game.time) * 60.0f;
        if (game.delta > 3)
            game.delta = 3;
        game.time = GetTime();
        for (size_t i = 0; i < this->clients.size(); i++) {
            if (FD_ISSET(this->clients[i]->getFd(), &this->readfds))
                getCommand(i);
        }
        if (this->clients.size() == 0) {
            delete wall;
            delete ground;
            delete brick;
            delete bombe;
            closeServer();
            break;
        }
        if (game.state == SOLOMODE) {
            if (game.areAllEnemiesDead()) {
                game.level += 1;
                int index = 0;
                for (auto &p : game.getPlayers()) {
                    p->playing = true;
                    p->setPosition(game.setPlayerPos(index++));
                }
                game.addEnemie(game.level);
            }
        }
        game.removeEntities(clients.size());
    }
}
