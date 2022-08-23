/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "entities/Entity.hpp"
#include "struct.hpp"

class Player : public Entity {
    public:
        Player(std::string path, Vector3 pos = {0, 0, 0});
        Player(Entity &ent, Vector3 pos = {0, 0, 0});
        ~Player();

        float getSpeed();
        int getCapacity();
        int getPower();
        bool getGhost();
        int getExplodedTime();

        void addSpeed(float speed) { stats.speed += speed;}
        void addCapacity(int cap) { stats.capacity += cap;}
        void setGhost(bool ghost) { stats.ghost = ghost;}
        void addPower(int pow) { stats.power += pow;}

        int nbBomb = 1;
        float placeAt = -1.0f;
        bool playing = true;
        long ghostTime = -1.0f;
    protected:
        playerStats stats;
    private:
};

#endif /* !PLAYER_HPP_ */
