/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

class Server;

#include <entities/Block.hpp>
#include "entities/Bombe.hpp"
#include "entities/Player.hpp"
#include "entities/Enemies.hpp"
#include <ctime>

enum GameState {
    SOLOMODE,
    MULTIMODE,
    NOTLAUNCH
};
class Server;

class Game {
    public:
        Game();
        ~Game();
        void processBombe();
        void createExplosion(Bombe *bombe);
        bool checkExplosion(Cardinal, Vector3, int);
        void removeEntities(int = -1);
        void addEntity(Entity *newEntity);
        void addEntity(Entity *newEntity, Vector3 pos);
        void movePlayerAt(Vector3 position, Player *);
        Player *getPlayer(size_t index);
        size_t addPlayer(std::string texturePath, Vector3 positionStart = {0, 0, 0});
        size_t addPlayer(Player *newPlayer);
        void removePlayer(size_t index);
        std::vector<Entity *> getEntities() { return this->entities;}
        std::vector<Player *> getPlayers() { return this->players;}
        Vector3 setPlayerPos(size_t);
        void setPlayersPos(void);
        void checkIfPlayerIsInPowder();
        bool isReady() { return this->ready;}
        void setReady(bool t) { this->ready = t;}
        bool arePlayersAllDead();
        void processIA();
        bool areAllEnemiesDead();
        int areOnePlayerLeft();
        void setTimeGame();
        std::string getTimeGame();
        void addEnemie(size_t nbr);
        float time;
        float delta;
        GameState state = NOTLAUNCH;
        int level = 1;

        Server *server;
        Entity *playerE;
    protected:
    private:
        std::time_t timeStart;
        std::vector<Entity *> entities;
        std::vector<Player *> players;
        Entity *powder;
        Entity *speed;
        Entity *ghost;
        Entity *power;
        Entity *bomb;
        Vector3 newBonus = {-1, -1, -1};
        bool ready = false;
};

#endif /* !GAME_HPP_ */
