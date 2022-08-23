/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Explosion
*/

#include "entities/Explosion.hpp"

Explosion::Explosion(Vector3 pos, Cardinal card) : Entity(EXPLOSION_TEXTURE_PATH, pos)
{
    // TRANSFORM EN FONCTION DU CARD
    this->model.transform;
}

Explosion::~Explosion()
{
}
