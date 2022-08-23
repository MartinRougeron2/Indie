/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Powder
*/

#include "Bombe.hpp"

#ifndef POWDER_HPP_
#define POWDER_HPP_

class Powder : public Bombe {
    public:
        Powder(Entity &copy, Vector3 pos = {0, 0, 0}, float time = 0.5f, int power = 0);
        Powder(std::string path, Vector3 pos = {0, 0, 0}, float time = 0.5f, int power = 0);
        ~Powder();
        bool processPowder();
    protected:
    private:
};

#endif /* !POWDER_HPP_ */
