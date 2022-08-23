/*
** EPITECH PROJECT, 2021
** B-YEP-400-LYN-4-1-indiestudio-anthony.couhier
** File description:
** bomberman->cpp
*/

#include "Bomberman.hpp"
#include "entities/Entity.hpp"
#include <algorithm>
#include <string.h>
#include <sstream>
#include <iostream>

#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#define XBOX360_LEGACY_NAME_ID  "Xbox Controller"
#if defined(PLATFORM_RPI)
    #define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
    #define XBOX360_NAME_ID     "Xbox 360 Controller"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif
#define sizeWidthMap 25
#define sizeHeightMap 25
#define screenWidth 1920
#define screenHeight 1080

Bomberman::Bomberman(void)
{
    InitWindow(screenWidth, screenHeight, "Bomberman");
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    InitAudioDevice();
    this->camera = {0};
    this->server = nullptr;
    SetTargetFPS(60);
    camera.position = (Vector3){ -sizeWidthMap /2, 40.0f, sizeHeightMap / 2};
    camera.target = (Vector3){ sizeWidthMap / 2, 0.0f,  sizeHeightMap / 2, };
    camera.up = (Vector3){ 0.0f, 50.0f, 0.0f };
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(this->camera, CAMERA_CUSTOM);
    ToggleFullscreen();
    this->state = LOADING;
    this->running = true;
    this->ipAdress = "127.0.0.1";
    Image cat = LoadImage("./assets/images/forest.png");
    t = clock();
    SetExitKey(0);
    Image splash = LoadImage("./assets/images/splashScreen.png");
    ImageResize(&splash, screenWidth, screenHeight);
    loadingSprite = LoadTextureFromImage(splash);
    Image splash2 = LoadImage("./assets/images/splashScreen2.png");
    ImageResize(&splash2, screenWidth, screenHeight);
    loadingSprite2 = LoadTextureFromImage(splash2);
    ImageResize(&cat, screenWidth, screenHeight);
    refreshSprite = LoadTextureFromImage(cat);
}

Bomberman::~Bomberman()
{
    UnloadTexture(this->loadingSprite);
    UnloadTexture(this->loadingSprite2);
    UnloadTexture(this->refreshSprite);
    CloseWindow();
    CloseAudioDevice();
}

Entity *Bomberman::getEntity(size_t index) {
    try {
        return this->entities.at(index);
    } catch (const std::out_of_range e) {
        return NULL;
    }
}

void Bomberman::addEntity(Entity *newEntity)
{
    this->entities.push_back(newEntity);
}

void Bomberman::addEntity(Entity *newEntity, Vector3 pos)
{
    newEntity->setPosition(pos);
    this->entities.push_back(newEntity);
}

void Bomberman::drawEntities()
{
    for (auto &&e : this->entities) {
        if (e->getType() == POWDER)
            this->interface.playSound("soundExplosion");
        e->displayEntity();
    }
}

std::string Bomberman::fillKeyPress()
{
    std::string res;

    if (IsKeyDown(this->option.right))
        res += RIGHT;
    if (IsKeyDown(this->option.left))
        res += LEFT;
    if (IsKeyDown(this->option.up))
        res += UP;
    if (IsKeyDown(this->option.down))
        res += DOWN;
    if (IsKeyReleased(this->option.bomb))
        res += BOMB;

    if (IsKeyDown(this->option.right2))
        res += RIGHT2;
    if (IsKeyDown(this->option.left2))
        res += LEFT2;
    if (IsKeyReleased(this->option.up2))
        res += UP2;
    if (IsKeyDown(this->option.down2))
        res += DOWN2;
    if (IsKeyDown(this->option.bomb2))
        res += BOMB2;
    // if (!IsGamepadAvailable(0) && (IsGamepadName(0, XBOX360_NAME_ID) || IsGamepadName(0, XBOX360_LEGACY_NAME_ID) || IsGamepadName(0, PS3_NAME_ID)))
    //     return res;

    // if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) )
    //     res += BOMB;

    // if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) > 1)
    //     res += RIGHT;
    // if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X) < 1)
    //     res += LEFT;
    // if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) > 1)
    //     res += UP;
    // if (GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y) < 1)
    //     res += DOWN;
    return res;
}

void Bomberman::run(void)
{
    bool launchServer = false;

    this->interface.setAssets();
    while (!WindowShouldClose() && this->running) {
        if (launchServer) {
            createServer();
            launchServer = false;
        }
        this->interface.setSizeScreen();
        this->interface.setMusic(this->state, this->option.mute, this->option.volume);
        switch (this->state) {
        case LOADING:
            splashScreen();
            break;
        case START:
            startMenu();
            break;
        case OPTION:
            optionMenu();
            break;
        case MULTI:
            multiMenu(launchServer);
            break;
        case SOLO:
            soloGame(launchServer);
            break;
        case GAME:
            game();
            break;
        }
    }
}

void Bomberman::splashScreen()
{
    std::string keyPress = fillKeyPress() + "\n";
    if (keyPress.compare("\n") != 0)
        changeState(START);
    BeginDrawing();
    t2 = clock();
    if (t2 - t > 30000) {
        DrawTexture(loadingSprite, 0, 0, WHITE);
        t = (t2 - t > 60000) ? clock() : t;
    } else
        DrawTexture(loadingSprite2, 0, 0, WHITE);
    EndDrawing();
}

void displayPlayerInformation(Interface &interface, const char *name, const char *stats, int x, int y)
{
    if (x < interface.sizeX / 2) {
        interface.displayImage("headBombermanRight", 0, x, y);

        interface.displayCircle((stats[0] == '1') ? GREEN : RED, 24, x + 100, y + 65);
        interface.displayText(name, 30, x + 25, y + 75, RAYWHITE);

        if (stats[0] != '1')
            return;
        y -= 25;
        x += 10;
        interface.displayImage("power", 0, x, y + (75 * 2));
        interface.displayText(TextFormat("%c\n", stats[1]), 30, x + 64, y + (75 * 2) + 25, RAYWHITE);

        interface.displayImage("bombe", 0, x, y + (75 * 3));
        interface.displayText(TextFormat("%c / %c", stats[3], stats[4]), 30, x + 64, y + (75 * 3) + 25, RAYWHITE);

        interface.displayImage("boots", 0, x, y + (75 * 4));
        interface.displayText(stats + 5, 30, x + 64, y + (75 * 4) + 25, RAYWHITE);

        if (stats[2] == '1')
            interface.displayImage("ghost", 0, x, y + (75 * 5) + 25);

    } else {
        interface.displayImage("headBombermanLeft", 0, x, y);
        interface.displayCircle((stats[0] == '1') ? GREEN : RED, 24, x - 50, y + 65);
        interface.displayText(name, 30, x - 75, y + 75, RAYWHITE);

        if (stats[0] != '1')
            return;
        y -= 25;
        x -= 10;
        interface.displayImage("power", 0, x, y + (75 * 2));
        interface.displayText(TextFormat("%c\n", stats[1]), 30, x - 64, y + (75 * 2) + 25, RAYWHITE);

        interface.displayImage("bombe", 0, x, y + (75 * 3));
        interface.displayText(TextFormat("%c / %c", stats[3], stats[4]), 30, x - 96, y + (75 * 3) + 25, RAYWHITE);

        interface.displayImage("boots", 0, x, y + (75 * 4));
        interface.displayText(stats + 5, 30, x - 64, y + (75 * 4) + 25, RAYWHITE);

        if (stats[2] == '1')
            interface.displayImage("ghost", 0, x, y + (75 * 5) + 25);
    }
}

void Bomberman::hudGame()
{
    char *tmp = this->client.sendAndReceive("getTime\n");

    this->interface.displayText(tmp, 30, this->interface.sizeX / 2, 10, RAYWHITE);
    free(tmp);
    tmp = this->client.sendAndReceive("getLevel\n");
    if (strcmp(tmp, "nothing") != 0)
        this->interface.displayText(TextFormat("level: %s", tmp), 30, this->interface.sizeX / 3, 10, RAYWHITE);
    free(tmp);
    tmp = this->client.sendAndReceive("getStat1\n");
    displayPlayerInformation(this->interface, "Player 1", tmp, 40, this->interface.sizeY / 10);

    free(tmp);
    if (this->client.nbPlayers == 2) {
        tmp = this->client.sendAndReceive("getStat2\n");
        displayPlayerInformation(this->interface, "Player 2", tmp, this->interface.sizeX - 40, this->interface.sizeY / 10);
        free(tmp);
    }
    if (IsKeyPressed(KEY_ESCAPE))
        changeState(OPTION);
}

void Bomberman::endGameHUD(char *data, bool &end)
{
    BeginDrawing();
    if (strcmp("1", data) == 0) {
        ClearBackground(GREEN);
        this->interface.displayText("WINNER", 40, this->interface.sizeX / 2, this->interface.sizeY / 2);
    } else {
        ClearBackground(RED);
        this->interface.displayText("LOSER", 40, this->interface.sizeX / 2, this->interface.sizeY / 2);
    }
    free(data);
    if (this->interface.displayButton("leaveButton", 10, this->interface.sizeX / 2, this->interface.sizeY / 3 * 2)) {
        this->client.doDisconnect();
        changeState(START);
        for (auto &i: this->entities)
            delete i;
        end = false;
        this->entities.clear();
    }
    EndDrawing();
}

void Bomberman::game(void)
{
    std::vector<std::string> data;
    std::string temp;
    static bool end = false;
    std::string keyPress = fillKeyPress() + "\n";
    char *res = NULL;

    free(res);
    res = client.sendAndReceive(keyPress.c_str());
    if (strcmp("none", res) != 0) {
        for (int i = 0; res[i] != 0; i++) {
            temp += res[i];
            if (res[i] == '\n') {
                data.push_back(temp);
                temp.clear();
            }
        }
        packDataEntity(data);
    }
    res = client.sendAndReceive("endGame\n");
    if (!end && (strcmp("0", res) == 0 || strcmp("1", res) == 0))
        end = true;
    if (end)
        return endGameHUD(res, end);
    BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTexture(refreshSprite, 0, 0, WHITE);
        BeginMode3D(camera);
            this->drawEntities();
        EndMode3D();
        hudGame();
    EndDrawing();
}

void Bomberman::packDataEntity(std::vector<std::string> data)
{
    for (size_t i = 0; i != data.size(); i++) {
        std::istringstream iss(data.at(i));
        std::string str;
        EntityData result;
        int index = 0;
        if (std::count(data[i].begin(), data[i].end(), ',') != 8)
            continue;
        while (std::getline(iss, str, ',')) {
            switch (index) {
                case 0:
                    try {
                        result.pos.x = std::stof(str);
                    } catch (std::invalid_argument) {
                        result.pos.x = 0;
                    }
                    break;
                case 1:
                    try {
                        result.pos.y = std::stof(str);
                    } catch (std::invalid_argument) {
                        result.pos.x = 0;
                    }
                    break;
                case 2:
                    try {
                        result.pos.z = std::stof(str);
                    } catch (std::invalid_argument) {
                        result.pos.x = 0;
                    }
                    break;
                case 3:
                    result.id = std::stoi(str);
                    break;
                case 4:
                    result.type = static_cast<EntityType>(std::stoi(str));
                    break;
                case 5:
                    result.facing = static_cast<Cardinal>(std::stoi(str));
                    break;
                case 6:
                    result.path = str;
                    break;
                case 7:
                    result.remove = std::stoi(str);
                    break;
                case 8:
                    str.pop_back();
                    result.scale = std::stof(str);
            }
            index++;
        }
        int found = 0;
        int indexE = 0;
        for (auto &e : this->getEntities()) {
            if (result.id == e->getID()) {
                e->setPosition(result.pos);
                e->setFacing(result.facing);
                if (result.remove) {
                    this->removeEntity(indexE);
                    indexE--;
                }
                found = 1;
                break;
            }
            indexE++;
        }
        int alreadyloaded = 0;
        if (not found) {
            for (auto &e : this->getEntities()) {
                if (result.path == e->getPath()) {
                    this->addEntity(new Entity(*e, result.pos, result.id, result.facing, result.type));
                    alreadyloaded = 1;
                    break;
                }
            }
            if (not alreadyloaded) {
                this->addEntity(new Entity(result.path, result.pos, true, true, result.scale, result.id, result.facing, result.type));
            }
        }
    }
}

void Bomberman::optionMenu()
{
    static int pageNb = 0;

    BeginDrawing();
        ClearBackground(SKYBLUE);
        if (this->stateSave == GAME && this->interface.displayButton("leaveButton", 20, this->interface.sizeX / 10 * 9, this->interface.sizeY / 22 * 19)) {
            this->client.doDisconnect();
            for (auto &i: this->entities)
                delete i;
            this->entities.clear();
            changeState(START);
        }
        if (this->interface.displayButton("leftArrowButton", 20, this->interface.sizeX / 10, this->interface.sizeY / 22 * 19) || IsKeyPressed(KEY_ESCAPE))
            changeState(this->stateSave);
        if (this->interface.displayButton("soundButton", 20, this->interface.sizeX / 5, this->interface.sizeY / 20, pageNb == 0))
            pageNb = 0;
        if (this->interface.displayButton("playerOneButton", 20, this->interface.sizeX / 2, this->interface.sizeY / 20, pageNb == 1))
            pageNb = 1;
        if (this->interface.displayButton("playerTwoButton", 20, this->interface.sizeX / 5 * 4, this->interface.sizeY / 20, pageNb == 2))
            pageNb = 2;

        switch (pageNb) {
        case 0:
            soundOption();
            break;
        case 1:
            playerOneOption();
            break;
        case 2:
            playerTwoOption();
            break;
        }
    EndDrawing();
}

void Bomberman::soundOption(void)
{
    if (!this->option.mute && this->interface.displayButton("volumeOnButton", 20, this->interface.sizeX / 3, this->interface.sizeY / 2))
        this->option.mute = true;
    else if (this->option.mute && this->interface.displayButton("volumeOffButton", 20, this->interface.sizeX / 3, this->interface.sizeY / 2))
        this->option.mute = false;
    this->option.volume = this->interface.guiSlideBar((Rectangle){
        static_cast<float>(this->interface.sizeX / 5 * 2),
        static_cast<float>(this->interface.sizeY / 2 + this->interface.getAssets("volumeOnButton")->sourceRec.height / 8),
        300,
        100
        }, this->option.volume, this->interface.getMouse());
}

void Bomberman::playerOneOption(void)
{
    static int selected = -1;

    this->interface.displayImage("upTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 2);
    this->interface.interactifText(1, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 2, this->option.up, selected);

    this->interface.displayImage("downTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 3);
    this->interface.interactifText(2, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 3, this->option.down, selected);

    this->interface.displayImage("rightTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 4);
    this->interface.interactifText(3, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 4, this->option.right, selected);

    this->interface.displayImage("leftTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 5);
    this->interface.interactifText(4, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 5, this->option.left, selected);

    this->interface.displayImage("bombTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 6);
    this->interface.interactifText(5, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 6, this->option.bomb, selected);

}

void Bomberman::playerTwoOption(void)
{
    static int selected = -1;

    this->interface.displayImage("upTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 2);
    this->interface.interactifText(6, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 2, this->option.up2, selected);

    this->interface.displayImage("downTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 3);
    this->interface.interactifText(7, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 3, this->option.down2, selected);

    this->interface.displayImage("rightTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 4);
    this->interface.interactifText(8, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 4, this->option.right2, selected);

    this->interface.displayImage("leftTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 5);
    this->interface.interactifText(9, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 5, this->option.left2, selected);

    this->interface.displayImage("bombTitle", 10, this->interface.sizeX / 5 * 2, this->interface.sizeY / 7 * 6);
    this->interface.interactifText(10, 10, this->interface.sizeX / 5 * 3, this->interface.sizeY / 7 * 6, this->option.bomb2, selected);
}

void Bomberman::startMenu(void)
{
    BeginDrawing();
        ClearBackground(SKYBLUE);
        this->interface.displayImage("logoBomberman", 20, this->interface.sizeX / 2, this->interface.sizeY / 20);
        if (this->interface.displayButton("soloButton", 50, this->interface.sizeX / 2, this->interface.sizeY / 5 * 2))
            changeState(SOLO);
        if (this->interface.displayButton("multiButton", 50, this->interface.sizeX / 2, (this->interface.sizeY / 5) * 2 + (131 + 10)))
            changeState(MULTI);
        if (this->interface.displayButton("settingsButton", 50, this->interface.sizeX / 2, (this->interface.sizeY / 5 * 2) + (131 * 2 + 20)))
            changeState(OPTION);
        if (this->interface.displayButton("exitButton", 50,  this->interface.sizeX / 10, this->interface.sizeY / 22 * 19) * 8)
            this->running = false;
    EndDrawing();
}

void Bomberman::createServer()
{
    if (fork() == 0) {
        this->server = new Server(8082);
        this->server->run();
        delete this->server;
        exit(0);
    }
}

void Bomberman::soloGame(bool &launchServer)
{
    if (!this->client.isConnected()) {
        static bool connection = false;
        static int setNbPlayer = 0;

        if (connection) {
            client.doConnect("127.0.0.1", setNbPlayer);
            this->client.nbPlayers = setNbPlayer;
            setNbPlayer = 0;
            connection = false;
            changeState(GAME);
            client.sendAndReceive("solo\n");
            return;
        }
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTexture(refreshSprite, 0, 0, WHITE);
        if (this->interface.displayButton("leftArrowButton", 20, this->interface.sizeX / 10, this->interface.sizeY / 22 * 19))
            changeState(START);
        if (this->interface.displayButton("onePlayerButton", 20, this->interface.sizeX / 2 / 3 * 2, this->interface.sizeY / 2, setNbPlayer == 1))
            setNbPlayer = 1;
        if (this->interface.displayButton("twoPlayersButton", 20, this->interface.sizeX / 2 * 3 / 2, this->interface.sizeY / 2, setNbPlayer == 2))
            setNbPlayer = 2;
        if (this->interface.displayButton("playButton", 20, this->interface.sizeX / 2, this->interface.sizeY / 3 * 2) && setNbPlayer) {
            launchServer = true;
            connection = true;
        }
        EndDrawing();
    }
}

void Bomberman::multiMenu(bool &launchServer)
{
    static int actionMultiMenu = 0;
    static bool toConnect = false;

    if (toConnect) {
        this->client.doConnect();
        toConnect = false;
    }
    switch (actionMultiMenu) {
    case 0:
        BeginDrawing();
        ClearBackground(SKYBLUE);
        this->interface.displayImage("logoBomberman", 20, this->interface.sizeX / 2, this->interface.sizeY / 20);
        if (this->interface.displayButton("leftArrowButton", 20, this->interface.sizeX / 10, this->interface.sizeY / 22 * 19))
            changeState(START);
        if (this->interface.displayButton("createMultiButton", 20, this->interface.sizeX / 2 / 3 * 2, this->interface.sizeY / 2)) {
            actionMultiMenu = 1;
            launchServer = true;
            toConnect = true;
        }
        if (this->interface.displayButton("joinMultiButton", 20, this->interface.sizeX / 2 * 3 / 2, this->interface.sizeY / 2))
            actionMultiMenu = 2;
        EndDrawing();
        break;

    case 1:
        createGame(actionMultiMenu);
        break;

    case 2:
        joinGame(actionMultiMenu);
        break;
    }
}

void Bomberman::createGame(int &actionMultiMenu)
{
    char *strPlayerConnected = this->client.sendAndReceive("nbConnected\n");
    char *strPlayerReady = this->client.sendAndReceive("nbReady\n");
    int nbPlayerConnected = std::stoi(strPlayerConnected);
    int nbPlayerReady = std::stoi(strPlayerReady);
    static bool isReady = false;

    free(strPlayerConnected);
    free(strPlayerReady);
    BeginDrawing();
    ClearBackground(SKYBLUE);

    if (this->interface.displayButton("leftArrowButton", 20, this->interface.sizeX / 10, this->interface.sizeY / 22 * 19)) {
        this->client.doDisconnect();
        actionMultiMenu = 0;
    }

    this->interface.displayText(FormatText("%d players connected", nbPlayerConnected), 20, this->interface.sizeX / 2, this->interface.sizeY / 4);
    this->interface.displayText(FormatText("%d players ready", nbPlayerReady), 20, this->interface.sizeX / 2, this->interface.sizeY / 3);

    if (this->interface.displayButton("readyButton", 10, this->interface.sizeX / 2, this->interface.sizeY / 4 * 3, isReady == true)) {
        if (isReady) {
            free(this->client.sendAndReceive("clientNotReady\n"));
            isReady = false;
        } else {
            free(this->client.sendAndReceive("clientReady\n"));
            isReady = true;
        }
    }
    if (isReady && nbPlayerConnected != 1 && nbPlayerConnected == nbPlayerReady && this->interface.displayButton("launchButton", 10, this->interface.sizeX / 10 * 8, this->interface.sizeY / 4 * 3)) {
        free(client.sendAndReceive("multi\n"));
        changeState(GAME);
    }
    EndDrawing();
}

void Bomberman::joinGame(int &actionMultiMenu)
{
    static bool isReady = false;
    std::string tmp;

    BeginDrawing();
        ClearBackground(SKYBLUE);
        this->interface.displayImage("logoBomberman", 20, this->interface.sizeX / 2, this->interface.sizeY / 20);

        if (this->interface.displayButton("leftArrowButton", 20, this->interface.sizeX / 10, this->interface.sizeY / 22 * 19)) {
            this->client.doDisconnect();
            actionMultiMenu = 0;
        }
        if (!this->client.isConnected()) {
            this->interface.guiGetString("largeString", this->ipAdress, 20, this->interface.sizeX / 2, this->interface.sizeY / 2, 15);
            this->interface.displayText("Address ip to connect:", 25, this->interface.sizeX / 2, this->interface.sizeY / 5 * 2);
            if (this->interface.displayButton("connectButton", 20, this->interface.sizeX / 2, this->interface.sizeY / 3 * 2))
                this->client.doConnect(this->ipAdress.c_str());
        } else {
            if (isReady) {
                tmp = this->client.sendAndReceive("multiLaunch\n");
                if (tmp == "LAUNCH") {
                    changeState(GAME);
                    actionMultiMenu = 0;
                    isReady = false;
                }
                this->interface.displayImage("waitMaster", 20, this->interface.sizeX / 2, this->interface.sizeY / 2);
            }
            if (this->interface.displayButton("readyButton", 10, this->interface.sizeX / 2, this->interface.sizeY / 4 * 3, isReady == true)) {
                if (isReady) {
                    free(this->client.sendAndReceive("clientNotReady\n"));
                    isReady = false;
                } else {
                    free(this->client.sendAndReceive("clientReady\n"));
                    isReady = true;
                }
            }
        }
    EndDrawing();
}

void Bomberman::changeState(enum stateGame newState)
{
    this->stateSave = this->state;
    this->state = newState;
}
