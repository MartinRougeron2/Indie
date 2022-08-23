/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Enemies
*/

#include "entities/Enemies.hpp"
#include <cmath>
#include "entities/Block.hpp"
#include "entities/Entity.hpp"
#include "iostream"
#include "raylib.h"
#include <unistd.h>

Enemies::Enemies(Entity &copy, Vector3 pos, float speed) : Entity(copy)
{
    this->type = ENEMIE;
    this->pos = pos;
    this->speed = speed;
}

Enemies::~Enemies()
{
}

Cardinal getInvertedCardinal(Cardinal card)
{
    if (card == NORTH)
        return SOUTH;
    if (card == SOUTH)
        return NORTH;
    if (card == EAST)
        return WEST;
    if (card == WEST)
        return EAST;
    return SOUTH;
}

Cardinal chooseDirection(std::vector<Cardinal> vec, Cardinal old)
{
    int select;

    if (vec.size() == 2) {
        select = rand() % 2;
        return vec[select];
    }
    if (vec.size() == 1)
        return vec[0];
    return getInvertedCardinal(old);
}

int Enemies::autoMove(Game *game)
{
    Vector3 move = getExplosedBlock({0}, this->getFacing(), this->speed * game->delta);
    Vector3 close;
    Cardinal oldface;
    Cardinal newface;
    Enemies cpy = *this;

    cpy.move(move);
    for (auto &e : game->getEntities()) {
        if (e->getType() == PLAYER && (*e && cpy)) {
            static_cast<Player *>(e)->playing = false;
            return 0;
        }
        if (e->getType() != ENEMIE && e->collision && (*e && cpy)) {
            if (this->placeAt > 0 && this->placeAt + 1.0f < GetTime()) {
                this->nbBomb += 1;
                this->placeAt = -1.0f;
            }
            if (e->getType() == BLOCK && static_cast<Block *>(e)->getIsDestructable() && this->nbBomb && std::rand() % 5 == 2) {
                return 1;
            }

            oldface = this->getFacing();
            std::vector<Cardinal> tested;
            for (int i = 0; i != 4; i++){
                newface = static_cast<Cardinal>(i);
                if (newface == oldface || newface == getInvertedCardinal(oldface))
                    continue;
                cpy.move(getExplosedBlock({0, 0, 0}, newface, 0.5));
                for (auto &e : game->getEntities()) {
                    if (e->getType() != ENEMIE && (e->collision && (*e && cpy))) {
                        tested.push_back(newface);
                        break;
                    }
                }
            }
            newface = chooseDirection(tested, oldface);
            this->setFacing(newface);
            return 0;
        }
    }
    this->move(move);
    return 0;
}
