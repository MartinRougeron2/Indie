/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** Bomberman.hpp
*/


#pragma once

class Server;

#include <algorithm>
#include "server/Server.hpp"
#include "entities/Bombe.hpp"
#include "entities/Player.hpp"
#include "Client.hpp"
#include "Option.hpp"
#include "interface/Interface.hpp"

Vector3 Vector3Scale(Vector3 vector, float Scale, Vector3 pos);

enum stateGame {
    LOADING,
    START,
    OPTION,
    SOLO,
    MULTI,
    GAME
};

class Bomberman {
    public:
        Bomberman();
        ~Bomberman();

        void splashScreen();
        void drawEntities();
        void run(void);
        void startMenu(void);
        void game(void);
        std::string fillKeyPress();

        void multiMenu(bool &launchServer);
        void createGame(int &actionMultiMenu);
        void joinGame(int &actionMultiMenu);
        void hudGame();
        void endGameHUD(char *data, bool &end);
        void soloGame(bool &launchServer);
        void createServer(void);

        void optionMenu();
        void soundOption(void);
        void playerOneOption(void);
        void playerTwoOption(void);

        void addEntity(Entity *newEntity);
        void addEntity(Entity *newEntity, Vector3 pos);

        void changeState(enum stateGame newState);
        void removeEntity(int index) { this->entities.erase(this->entities.begin() + index);}
        void packDataEntity(std::vector<std::string> data);

        Entity *getEntity(size_t index);
        std::vector<Entity *> getEntities() { return this->entities;}
        Camera3D camera;

    private:
        std::vector<Entity *> entities;
        std::string theme;
        enum stateGame state;
        enum stateGame stateSave;
        Client client;
        Server *server;
        Option option;
        int nbPlayer = 1;
        Interface interface;
        Texture2D refreshSprite;
        bool running;
        std::string ipAdress;
        Texture2D loadingSprite;
        Texture2D loadingSprite2;
        clock_t t;
        clock_t t2;
};
