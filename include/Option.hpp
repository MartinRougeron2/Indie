/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** option
*/

#ifndef OPTION_HPP_
#define OPTION_HPP_

enum input{
    RIGHT = 33,
    LEFT,
    UP,
    DOWN,
    BOMB,
    RIGHT2,
    LEFT2,
    UP2,
    DOWN2,
    BOMB2,
    NONE
};

class Option {
    public :
        Option();
        ~Option();

        void setMovement(input, int);

        input getInputType(int);


        int right = 'D';
        int left = 'A';
        int up = 'W';
        int down = 'S';
        int bomb = ' ';
        /* Fleche */
        int right2 = 262;
        int left2 = 263;
        int up2 = 265;
        int down2 = 264;
        int bomb2 = 257;
        float volume = 50.0f;
        bool mute = false;
    private :
};
#endif /* !OPTION_HPP_ */
