/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Bombe
*/

#ifndef BOMBE_HPP_
#define BOMBE_HPP_

#include "entities/Entity.hpp"

#define DEFAULT_EXPLODETIME 5
#define DEFAULT_BOMBEPOWER 1

class Bombe : public Entity {
    public:
        Bombe(std::string path, Vector3 pos = {0, 0, 0}, float time = DEFAULT_EXPLODETIME, int power = DEFAULT_BOMBEPOWER);
        Bombe(Entity &copy, Vector3 pos = {0, 0, 0}, float time = DEFAULT_EXPLODETIME, int power = DEFAULT_BOMBEPOWER);
        ~Bombe();
        bool processBombe();

        int getPower() const { return this->power;}

    protected:
        float explodetime;
        float placetime;
        int power;
};

#endif /* !BOMBE_HPP_ */
