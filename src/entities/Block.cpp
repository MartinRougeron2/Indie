/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Block
*/

#include "entities/Block.hpp"

Block::Block(Entity &copy, Vector3 pos, bool destructable) : Entity(copy)
{
    this->type = BLOCK;
    this->pos = pos;
    this->destructable = destructable;
}


Block::~Block()
{
}
