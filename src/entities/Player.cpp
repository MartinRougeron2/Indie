/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Player
*/

#include "entities/Player.hpp"

Player::Player(std::string path, Vector3 pos) : Entity(path, pos)
{
    this->type = PLAYER;
    collision = false;
    playing = true;
    stats = {0};
    stats.speed = 0.2;
    stats.capacity = 1;
    stats.power = 3;
    stats.ghost = false;
    stats.explosiontime = 3;
}

Player::Player(Entity &ent, Vector3 pos) : Entity(ent, pos)
{
    this->type = PLAYER;
    collision = false;
    playing = true;
    stats.speed = 0.2;
    stats.capacity = 1;
    stats.power = 3;
    stats.ghost = false;
    stats.explosiontime = 3;
}

Player::~Player()
{
}

float Player::getSpeed() {
    return stats.speed;
}

int Player::getCapacity() {
    return stats.capacity;
}

int Player::getPower() {
    return stats.power;
}

bool Player::getGhost() {
    return stats.ghost;
}

int Player::getExplodedTime() {
    return stats.explosiontime;
}
