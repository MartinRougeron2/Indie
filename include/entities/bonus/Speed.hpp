/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Speed
*/

#ifndef SPEED_HPP_
#define SPEED_HPP_

#include "entities/bonus/Bonus.hpp"
#define SPEEDADD 0.05f

class Speed : public Bonus {
    public:
        Speed(Entity &copy, Vector3 pos) : Bonus(copy, pos) {};
        ~Speed();
        void useBonus(Player *player) { player->addSpeed(SPEEDADD);}

    protected:
    private:
};

#endif /* !SPEED_HPP_ */
