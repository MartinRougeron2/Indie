/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** main
*/

#include "Bomberman.hpp"
#include <signal.h>

int main(void)
{
    Bomberman bomberman;

    signal(SIGCHLD, SIG_IGN);
    bomberman.run();
    return 0;
}
