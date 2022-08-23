/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Option
*/

#include "Option.hpp"

Option::Option()
{
    this->mute = false;
}

Option::~Option()
{
}

void Option::setMovement(input _input, int c)
{
    if (_input == RIGHT)
        this->right = c;
    if (_input == LEFT)
        this->left = c;
    if (_input == UP)
        this->up = c;
    if (_input == DOWN)
        this->down = c;
    if (_input == BOMB)
        this->bomb = c;
    if (_input == RIGHT2)
        this->right2 = c;
    if (_input == LEFT2)
        this->left2 = c;
    if (_input == UP2)
        this->up2 = c;
    if (_input == DOWN2)
        this->down2 = c;
    if (_input == BOMB2)
        this->bomb2 = c;
}

input Option::getInputType(int key)
{
    if (key == this->right)
        return RIGHT;
    if (this->left == key)
        return LEFT;
    if (this->up == key)
        return UP;
    if (this->down == key)
        return DOWN;
    if (this->bomb == key)
        return BOMB;
    if (this->right2 == key)
        return RIGHT2;
    if (this->left2 == key)
        return LEFT2;
    if (this->up2 == key)
        return UP2;
    if (this->down2 == key)
        return DOWN2;
    if (this->bomb2 == key)
        return BOMB2;
    return NONE;
}
