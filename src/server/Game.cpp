/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Game
*/

#include "server/Game.hpp"
#include "entities/Entity.hpp"
#include "entities/bonus/Bonus.hpp"
#include "entities/bonus/Capacity.hpp"
#include "entities/bonus/Ghost.hpp"
#include "entities/bonus/Power.hpp"
#include "entities/bonus/Speed.hpp"
#include "entities/Powder.hpp"
#include "server/Server.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <stdio.h>

Game::Game()
{
    std::string waterp("assets/model/lava01.obj");
    std::string snowp("assets/model/wallStone.obj");
    std::string bomb("assets/model/tmp/bomb.obj");
    std::string ghost("assets/model/tmp/ghost.obj");
    std::string power("assets/model/tmp/power.obj");
    std::string speed("assets/model/tmp/speed.obj");

    this->powder = new Entity(waterp, {0}, false, true);
    this->powder->collision = false;
    this->ready = false;
    this->playerE = new Entity("assets/model/bomberman.obj", {0}, false, true, 0.005f);
    this->speed = new Entity(speed, {0}, false, true, 0.008f);
    this->power = new Entity(power, {0}, false, true, 0.008f);
    this->ghost = new Entity(ghost, {0}, false, true, 0.008f);
    this->bomb = new Entity(bomb, {0}, false, true, 0.008f);
}

Game::~Game()
{
}

Vector3 getExplosedBlock(Vector3 pos, Cardinal card, float pow)
{
    switch (card) {
        case NORTH:
            return (Vector3) {pos.x + pow, pos.y, pos.z};
        case SOUTH:
            return (Vector3) {pos.x - pow, pos.y, pos.z};
        case WEST:
            return (Vector3) {pos.x, pos.y, pos.z - pow};
        case EAST:
            return (Vector3) {pos.x, pos.y, pos.z + pow};
        default:
            return (Vector3) {0, 0, 0};
    }
}

bool Game::checkExplosion(Cardinal card, Vector3 bombePos, int y)
{
    Vector3 explodedPos = getExplosedBlock(bombePos, card, y);
    Powder *newPowder = new Powder(*this->powder);
    bool block;

    for (auto &e : this->entities) {
        // ? Entity have same pos as bomb
        if (*e && explodedPos) {
            if (e->getType() == BLOCK && static_cast<Block *>(e)->getIsDestructable()) {
                e->setToRemove(1);
                e->modified.clear();
                newPowder->setPosition(explodedPos);
                this->entities.push_back(newPowder);
                block = true;
                this->newBonus = e->getPosition();
            } else if (e->getType() == BLOCK && !static_cast<Block *>(e)->getIsDestructable())
                block = true;
            return block;
        }
    }
    newPowder->setPosition(explodedPos);
    this->entities.push_back(newPowder);
    return false;
}

void Game::checkIfPlayerIsInPowder()
{
    std::vector<Entity *> players;
    std::vector<Entity *> powders;

    for (auto &e : this->entities) {
        switch (e->getType()) {
            case PLAYER:
            case ENEMIE:
                players.push_back(e);
                break;
            case POWDER:
                powders.push_back(e);
            case BLOCK:
                powders.push_back(e);
            default:
                ;
        }
    }
    int i = 0;
    for (auto &player : players) {
        for (auto &powder : powders) {
            if (*powder && *player) {
                if (player->getType() == PLAYER)
                    static_cast<Player *>(player)->playing = false;
                if (player->getType() == ENEMIE) {
                    static_cast<Enemies *>(player)->setToRemove(true);
                    static_cast<Enemies *>(player)->modified.clear();
                }
                break;
            }
        }
    }
}

void Game::createExplosion(Bombe *bombe)
{
    Powder *newPowder = new Powder(*this->powder);

    newPowder->setPosition(bombe->getPosition());
    this->entities.push_back(newPowder);
    for (int i = 0; i != 4; i++) {
        for (int y = 0; y != bombe->getPower(); y++) {
            auto bombePos = bombe->getPosition();
            if (checkExplosion(static_cast<Cardinal>(i), bombePos, y + 1)) {
                if (this->newBonus.x < 0)
                    break;
                int r = std::rand() % 60;

                switch (r) {
                    case 0:
                        this->entities.push_back(new Capacity(*bomb, newBonus));
                        break;
                    case 1:
                        this->entities.push_back(new Ghost(*ghost, newBonus));
                        break;
                    case 2:
                        this->entities.push_back(new Power(*power, newBonus));
                        break;
                    case 3:
                        this->entities.push_back(new Speed(*speed, newBonus));
                        break;
                    default:
                        break;
                }
                this->newBonus.x = -1;
                break;
            }
        }
    }
    checkIfPlayerIsInPowder();
}

void Game::processBombe()
{
    for (auto &e : this->entities) {
        if (e->getType() == POWDER && not e->getToRemove()) {
            Powder *powder = static_cast<Powder *>(e);
            powder->processPowder();
            continue;
        }
        if (e->getType() == PLAYER && static_cast<Player *>(e)->getGhost())
            if (static_cast<Player *>(e)->ghostTime + 9.0f < GetTime()) {
                static_cast<Player *>(e)->ghostTime = -1.0f;
                static_cast<Player *>(e)->setGhost(false);
                checkIfPlayerIsInPowder();
            }
        if (e->getType() != BOMBE || e->getToRemove())
            continue;
        Bombe *bombe = static_cast<Bombe *>(e);
        if (bombe->processBombe()) {
            this->createExplosion(bombe);
        }
    }

    for (auto &p : this->players) {
        if (p->placeAt < 0 || p->nbBomb == p->getCapacity())
            continue;
        if (p->placeAt + p->getExplodedTime() < GetTime()) {
            p->nbBomb += 1;
            if (p->nbBomb != p->getCapacity())
                p->placeAt = GetTime();
            else
                p->placeAt = -1.0f;
        }
    }
}

void Game::removeEntities(int client)
{
    this->entities.erase(std::remove_if(this->entities.begin(), this->entities.end(),
    [client](Entity *e)
    {
        if (e->modified.size() == client && e->getToRemove())
            return 1;
        return 0;
    }), this->entities.end());
}

void Game::movePlayerAt(Vector3 position, Player *player) {
    Player cpy = *player;

    cpy.setFacing(position);
    cpy.move(position);
    for (auto &e : this->entities) {
        if (*e && cpy) {
            if (e->getType() == BONUS) {
                static_cast<Bonus *>(e)->useBonus(player);
                e->modified.clear();
                e->setToRemove(1);
            }
            if (e->getType() != PLAYER && e->collision) {
                if (e->getType() == BLOCK && (static_cast<Block *>(e))->getIsDestructable() && player->getGhost())
                    continue;
                return;
            }
        }
    }
    player->setFacing(position);
    player->move(position);
}

void Game::addEntity(Entity *newEntity)
{
    this->entities.push_back(newEntity);
}

void Game::addEntity(Entity *newEntity, Vector3 pos)
{
    newEntity->setPosition(pos);
    this->entities.push_back(newEntity);
}

size_t Game::addPlayer(std::string texturePath, Vector3 positionStart)
{
    Player *newPlayer = new Player(texturePath, positionStart);

    this->entities.push_back(newPlayer);
    this->players.push_back(newPlayer);
    return this->players.size() - 1;
}

size_t Game::addPlayer(Player *newPlayer)
{
    this->entities.push_back(newPlayer);
    this->players.push_back(newPlayer);
    return this->players.size() - 1;
}

void Game::removePlayer(size_t index)
{
    Player *tmp = this->players[index];

    for (size_t i = 0; i < this->entities.size(); i++) {
        if (this->entities[i] == tmp) {
            this->entities.erase(this->entities.begin() + i);
            break;
        }
    }
    this->players.erase(this->players.begin() + index);
    delete tmp;
}

Player *Game::getPlayer(size_t index)
{
    try {
        return this->players.at(index);
    }
    catch(const std::out_of_range &e) {
        return NULL;
    }
}

bool Game::arePlayersAllDead()
{
    size_t playerDead = 0;

    for (auto &e : players) {
        if (e->getType() == PLAYER) {
            auto p = static_cast<Player *>(e);
            if (!p->playing)
                playerDead++;
        }
    }
    return (playerDead == this->players.size());
}

Vector3 Game::setPlayerPos(size_t index)
{
    Vector2 mapSize = {25, 25};

    switch (index) {
        case 0:
            return {2.0f, 1.0f, 2.0f};
        case 1:
            return {2.0f, 1.0f, mapSize.x - 2.0f};
        case 2:
            return {mapSize.y - 2.0f, 1.0f, 2.0f};
        case 3:
            return {mapSize.y - 2.0f, 1.0f, mapSize.x - 2.0f};
        default:
            return {0, 0, 0};
    }
}

void Game::processIA()
{
    for (auto &p : this->entities) {
        if (p->getType() == ENEMIE) {
            if (static_cast<Enemies *>(p)->autoMove(this))
                server->placeBombeE(static_cast<Enemies *>(p));
        }
    }
}

bool Game::areAllEnemiesDead()
{
    for (auto &e : this->entities) {
        if (e->getType() == ENEMIE)
            return false;
    }
    return true;
}

int Game::areOnePlayerLeft()
{
    size_t playerLeft = 0;
    int index = -1;
    int i = 0;

    for (auto &p : players) {
        if (p->playing) {
            ++playerLeft;
            index = i;
        }
        ++i;
    }
    if (playerLeft == 1)
        return index;
    return -1;
}

void Game::setTimeGame()
{
    this->timeStart = std::time(NULL);
}

std::string Game::getTimeGame()
{
    char str[32] = {0};
    std::time_t t = std::time(NULL) - this->timeStart;
    int minute = (t  % 3600) / 60;
    int seconds = t  % 60;

    sprintf(str, "%02d:%02d", minute, seconds);
    return str;
}

void Game::setPlayersPos(void)
{
    for (size_t i = 0; i < this->players.size(); i++)
        this->players[i]->setPosition(setPlayerPos(i));
}

void Game::addEnemie(size_t nbr)
{
    int placed = 0;
    std::vector<Entity *> line;

    while (nbr > 0) {
        for (auto &e : this->entities) {
            if (e->getPosition().x == (23 - placed * 3) && e->getPosition().y == 1)
                line.push_back(e);
        }
        for (float i = rand() % 12; i != 24; i++) {
            bool find = false;
            for (auto &e : line) {
                if ((*e && (Vector3) {23 - placed * 3.0f, 1, i})) {
                    find = true;
                    break;
                }
            }
            if (find == false) {
                placed++;
                nbr -= 1;
                this->addEntity(new Enemies(*this->playerE, (Vector3) {23 - placed * 3.0f, 1.0f, i}));
                line.clear();
                break;
            }
        }
        line.clear();
    }
}
