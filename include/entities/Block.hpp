/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Block
*/

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include "entities/Entity.hpp"

class Block : public Entity {
    public:
        Block(Entity &copy, Vector3 pos, bool destructable = false);
        ~Block();

        bool getIsDestructable() const { return this->destructable;}
    protected:
    private:
        bool destructable;
};

#endif /* !BLOCK_HPP_ */