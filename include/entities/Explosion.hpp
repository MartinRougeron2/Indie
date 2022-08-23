/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Explosion
*/

#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "entities/Entity.hpp"
#include "Bomberman.hpp"

#define EXPLOSION_TEXTURE_PATH std::string("assets/model/stone02.obj")

class Explosion : public Entity {
    public:
        Explosion(Vector3 pos = {0, 0, 0}, Cardinal card = NORTH);
        ~Explosion();

    protected:
    private:
};

#endif /* !EXPLOSION_HPP_ */
