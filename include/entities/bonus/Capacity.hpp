/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Capacity
*/

#ifndef CAPACITY_HPP_
#define CAPACITY_HPP_

#include "entities/bonus/Bonus.hpp"

class Capacity : public Bonus{
    public:
        Capacity(Entity &copy, Vector3 pos) : Bonus(copy, pos) {};
        ~Capacity();
        void useBonus(Player *player) { player->addCapacity(1);}

    protected:
    private:
};

#endif /* !CAPACITY_HPP_ */
