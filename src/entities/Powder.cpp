/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Powder
*/

#include "entities/Powder.hpp"

Powder::Powder(Entity &copy, Vector3 pos, float time, int power) : Bombe(copy, pos, time, power)
{
    this->explodetime = time;
    this->power = 0;
    this->type = POWDER;
}

Powder::Powder(std::string path, Vector3 pos, float time, int power) : Bombe(path, pos, time, power)
{
    this->explodetime = time;
    this->power = 0;
    this->type = POWDER;
}

Powder::~Powder()
{
}

bool Powder::processPowder()
{
    if (this->placetime + this->explodetime < GetTime()) {
        this->toRemove = 1;
        this->modified.clear();
        return true;
    }
    return false;
}
