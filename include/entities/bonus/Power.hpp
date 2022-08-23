/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Power
*/

#ifndef POWER_HPP_
#define POWER_HPP_

#include "entities/bonus/Bonus.hpp"

class Power : public Bonus{
    public:
        Power(Entity &copy, Vector3 pos) : Bonus(copy, pos) {};
        ~Power();
        void useBonus(Player *player) { player->addPower(1);}

    protected:
    private:
};

#endif /* !POWER_HPP_ */
