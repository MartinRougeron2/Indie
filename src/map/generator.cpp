/*
** EPITECH PROJECT, 2021
** Dante-s-star
** File description:
** generator
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <experimental/random>
#include <string.h>

std::vector<std::string> generator(int x, int y)
{
    std::vector<std::string> map;
    std::string line;

    for (int i = 0; i < y; i++) {
        for (int e = 0; e < x; e++) {
            if (i == 0 || i == y - 1 || e == 0 || e == x -1)
                line.push_back('1');
            else {
                if (((i == 1 || i == 2) && (e == 1 || e == 2)) ||  ((i == 1 || i == 2)
                && (e == x - 2 || e == x - 3)) || ((i == y - 2 || i == y - 3) && (e == 1 || e == 2))
                || ((i == y - 2 || i == y - 3) && (e == x - 2 || e == x - 3)))
                    line.push_back('0');
                else {
                    int random = std::experimental::randint(0, 4);
                    if (random == 0 || random == 3 || random == 4)
                        line.push_back('0');
                    if (random == 1) {
                        if (map[i - 1][e] == '1' || map[i - 1][e - 1] == '1'
                        || map[i - 1][e + 1] == '1' || line.back() == '1')
                            line.push_back('2');
                        else
                            line.push_back('1');
                    }
                    if (random == 2)
                        line.push_back('2');
                }
            }
        }
    map.push_back(line);
    line.clear();
    }
    return map;
}