/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Enemies
*/

#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_

class Game;

#include "server/Game.hpp"
#include "entities/Entity.hpp"
#define defaultEnemiesSpeed 0.07f

Vector3 getExplosedBlock(Vector3 pos, Cardinal card, float pow);


class Enemies : public Entity {
    public:
        Enemies(Entity &copy, Vector3 pos, float speed = defaultEnemiesSpeed);
        ~Enemies();

        int autoMove(Game *game);
        int getPower() {return this->power;}

        float speed;
        int power = 3;
        bool alive = true;
        int nbBomb = 1;
        float placeAt = -1.0f;
    protected:
    private:
};

#endif /* !ENEMIES_HPP_ */
