/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Bonus
*/

#ifndef BONUS_HPP_
#define BONUS_HPP_
#include "entities/Player.hpp"
#include "entities/Entity.hpp"

class Bonus : public Entity{
    public:
        Bonus(Entity &copy, Vector3 pos) : Entity(copy, pos) { this->type = BONUS;};
        ~Bonus();
        virtual void useBonus(Player *player) {};

    protected:
    private:
};

#endif /* !BONUS_HPP_ */
