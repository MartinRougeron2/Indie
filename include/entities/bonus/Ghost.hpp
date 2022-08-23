/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Ghost
*/

#ifndef GHOST_HPP_
#define GHOST_HPP_

#include "entities/bonus/Bonus.hpp"

class Ghost : public Bonus{
    public:
        Ghost(Entity &copy, Vector3 pos) : Bonus(copy, pos) {};
        ~Ghost();
        void useBonus(Player *player) { 
            player->ghostTime = GetTime();
            player->setGhost(true);
        }

    protected:
        float countdown;
        float placetime;
    private:
};

#endif /* !GHOST_HPP_ */
