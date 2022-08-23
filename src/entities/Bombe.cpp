/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Bombe
*/

#include "entities/Bombe.hpp"
#include "iostream"

Bombe::Bombe(std::string path, Vector3 pos, float time, int power) : Entity(path, pos)
{
    this->explodetime = time;
    this->placetime = GetTime();
    this->type = BOMBE;
    this->collision = false;
}

Bombe::Bombe(Entity &copy, Vector3 pos, float time, int power) : Entity(copy)
{
    this->explodetime = time;
    this->placetime = GetTime();
    this->type = BOMBE;
    this->power = power;
    this->pos = pos;
    this->collision = false;
}

Bombe::~Bombe()
{
}

bool Bombe::processBombe()
{
    if (this->placetime + this->explodetime < GetTime()) {
        this->toRemove = 1;
        this->modified.clear();
        return true;
    }
    return false;
}
