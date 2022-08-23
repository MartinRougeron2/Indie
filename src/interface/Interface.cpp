/*
** EPITECH PROJECT, 2021
** indiestudio
** File description:
** Interface
*/

#include "interface/Interface.hpp"
#include <iostream>

Interface::Interface(void)
{
    this->sizeX = 0;
    this->sizeY = 0;
}

Interface::~Interface()
{
    for (auto i: this->sound)
        UnloadMusicStream(i.second);
    for (auto &i: this->button) {
        UnloadTexture(i.second->texture);
        UnloadImage(i.second->data);
        delete i.second;
    }
}

static struct dataAssets *setDataAssets(const char *path)
{
    struct dataAssets *data = new struct dataAssets;

    data->texture = LoadTexture(path);
    data->data = GetTextureData(data->texture);
    data->sourceRec = {0, 0, static_cast<float>(data->data.width / 2), static_cast<float>(data->data.height)};
    return data;
}

void Interface::setAssets(void)
{
    this->font = LoadFont("assets/fonts/Komikax.ttf");
    this->button = {
        {"connectButton", setDataAssets("assets/images/button/connectButton.png")},
        {"createMultiButton", setDataAssets("assets/images/button/createMultiButton.png")},
        {"exitButton", setDataAssets("assets/images/button/exitButton.png")},
        {"helpButton", setDataAssets("assets/images/button/helpButton.png")},
        {"joinMultiButton", setDataAssets("assets/images/button/joinMultiButton.png")},
        {"leftArrowButton", setDataAssets("assets/images/button/leftArrowButton.png")},
        {"menuButton", setDataAssets("assets/images/button/menuButton.png")},
        {"keyButton", setDataAssets("assets/images/button/keyButton.png")},
        {"launchButton", setDataAssets("assets/images/button/launchButton.png")},
        {"leaveButton", setDataAssets("assets/images/button/leaveButton.png")},
        {"multiButton", setDataAssets("assets/images/button/multiButton.png")},
        {"onePlayerButton", setDataAssets("assets/images/button/onePlayerButton.png")},
        {"playButton", setDataAssets("assets/images/button/playButton.png")},
        {"playerOneButton", setDataAssets("assets/images/button/playerOneButton.png")},
        {"playerTwoButton", setDataAssets("assets/images/button/playerTwoButton.png")},
        {"readyButton", setDataAssets("assets/images/button/readyButton.png")},
        {"rightArrowButton", setDataAssets("assets/images/button/rightArrowButton.png")},
        {"settingsButton", setDataAssets("assets/images/button/settingsButton.png")},
        {"soloButton", setDataAssets("assets/images/button/soloButton.png")},
        {"soundButton", setDataAssets("assets/images/button/soundButton.png")},
        {"twoPlayersButton", setDataAssets("assets/images/button/twoPlayersButton.png")},
        {"volumeOffButton", setDataAssets("assets/images/button/volumeOffButton.png")},
        {"volumeOnButton", setDataAssets("assets/images/button/volumeOnButton.png")}
    };
    this->title = {
        {"bombe", setDataAssets("assets/images/title/bombe.png")},
        {"boots", setDataAssets("assets/images/title/boots.png")},
        {"ghost", setDataAssets("assets/images/title/ghost.png")},
        {"power", setDataAssets("assets/images/title/power.png")},
        {"downTitle", setDataAssets("assets/images/title/downTitle.png")},
        {"headBombermanLeft", setDataAssets("assets/images/title/headBombermanLeft.png")},
        {"headBombermanRight", setDataAssets("assets/images/title/headBombermanRight.png")},
        {"largeString", setDataAssets("assets/images/title/largeString.png")},
        {"leftTitle", setDataAssets("assets/images/title/leftTitle.png")},
        {"logoBomberman", setDataAssets("assets/images/title/logoBomberman.png")},
        {"rightTitle", setDataAssets("assets/images/title/rightTitle.png")},
        {"upTitle", setDataAssets("assets/images/title/upTitle.png")},
        {"bombTitle", setDataAssets("assets/images/title/bombTitle.png")},
        {"waitMaster", setDataAssets("assets/images/title/waitMaster.png")}
    };
    this->sound = {
        {"musicMenu", LoadMusicStream("assets/music/menu.mp3")},
        {"inGame", LoadMusicStream("assets/music/inGame.mp3")},
        {"soundButton", LoadMusicStream("assets/music/soundButton.wav")},
        {"soundExplosion", LoadMusicStream("assets/music/bombExplosion.ogg")}
    };
    this->sound["soundButton"].looping = false;
    this->sound["soundExplosion"].looping = false;
    PlayMusicStream(this->sound["musicMenu"]);
}

void Interface::setSizeScreen(void)
{
    this->sizeX = GetScreenWidth();
    this->sizeY = GetScreenHeight();
    this->mouse = GetMousePosition();
}

void Interface::setMusic(int state, bool mute, int volume)
{
    if (state != 5) {
        if (!IsMusicStreamPlaying(this->sound["musicMenu"]))
            PlayMusicStream(this->sound["musicMenu"]);
        UpdateMusicStream(this->sound["musicMenu"]);
    } else {
        if (!IsMusicStreamPlaying(this->sound["inGame"]))
            PlayMusicStream(this->sound["inGame"]);
        UpdateMusicStream(this->sound["inGame"]);
    }

    if (IsMusicStreamPlaying(this->sound["soundButton"]))
        UpdateMusicStream(this->sound["soundButton"]);

    if (IsMusicStreamPlaying(this->sound["soundExplosion"]))
        UpdateMusicStream(this->sound["soundExplosion"]);
    for (auto &i: this->sound)
        SetMusicVolume(i.second, (mute ? 0 : volume) / 100.0f);
}

void Interface::playSound(const char *sound)
{
    PlayMusicStream(this->sound[sound]);
}

bool Interface::displayButton(const char *image, float pourcent, float x, float y, bool select)
{
    struct dataAssets *button = this->button[image];
    Rectangle tmp = button->sourceRec;
    bool retValue = false;

    centerVarToPosition<int, float>(button->sourceRec.width, x);
    if (CheckCollisionPointRec(this->mouse, (Rectangle){x, y, button->sourceRec.width, button->sourceRec.height})) {
        tmp.x = button->sourceRec.width;

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            y += 2;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            PlayMusicStream(this->sound["soundButton"]);
            retValue = true;
        }
    }
    if (select) {
        tmp.x = button->sourceRec.width;
    }
    DrawTextureRec(button->texture, tmp, (Vector2){x, y}, RAYWHITE);
    return retValue;
}

void Interface::displayImage(const char *image, float pourcent, float x, float y)
{
    struct dataAssets *button = this->title[image];

    centerVarToPosition<int, float>(button->data.width, x);
    DrawTextureV(button->texture, (Vector2){x, y}, RAYWHITE);
}

void Interface::displayText(const char *text, int sizeFont, int x, int y, Color c)
{
    Vector2 sizeText = MeasureTextEx(this->font, text, sizeFont, 0);

    centerVarToPosition<float, int>(sizeText.x, x);
    DrawText(text, x, y, sizeFont, c);
}

Vector2 Interface::getMouse() const
{
    return this->mouse;
}

int Interface::guiSlideBar(Rectangle size, float actualValue, Vector2 mouse, float endValue)
{
    Rectangle slideBar = {size.x + 4, size.y + 4, 0, size.height - 8};
    Vector2 posText = {slideBar.x + 8, size.y};

    actualValue = actualValue / 100 * size.width;
    if (CheckCollisionPointRec(mouse, size) && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        actualValue = mouse.x - size.x;
    slideBar.width = actualValue - 6;
    DrawRectangleLinesEx(size, 2, BLACK);
    DrawRectangleRec(slideBar, DARKGRAY);
    DrawTextEx(this->font, TextFormat("%02.00f", actualValue / size.width * 100), posText, slideBar.height, 0, BLACK);

    return actualValue / size.width * 100;
}

dataAssets *Interface::getAssets(const char *a)
{
    if (this->button.find(a) != this->button.end())
        return this->button[a];
    if (this->title.find(a) != this->title.end())
        return this->title[a];
    return nullptr;
}

void Interface::interactifText(int id, float pourcent, float x, float y, int &key, int &idSelect)
{
    struct dataAssets *button = this->button["keyButton"];
    int tmp = 0;
    Rectangle checkIdBox = {x, y, static_cast<float>(button->data.height), static_cast<float>(button->data.width)};
    float sizeFont = checkIdBox.height / 3;
    const char *keyToPrint = NULL;
    static std::map<int, const char *> keys = {
        {KEY_UP, "UP"},
        {KEY_DOWN, "DOWN"},
        {KEY_LEFT, "LEFT"},
        {KEY_RIGHT, "RIGHT"},
        {KEY_ENTER, "ENTER"},
        {' ', "SPACE"}
    };
    if (keys.find(key) != keys.end())
        keyToPrint = keys[key];
    else
        keyToPrint = TextFormat("%c", key);
    if (idSelect == id && (tmp = GetKeyPressed())) {
        idSelect = -1;
        key = tmp;
    }
    int textSizeX = MeasureTextEx(this->font, keyToPrint, sizeFont, 0).x / 2;
    int textSizeY = MeasureTextEx(this->font, keyToPrint, sizeFont, 0).y / 2;
    if (CheckCollisionPointRec(this->mouse, checkIdBox) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        DrawTextureV(button->texture, (Vector2){x, y + 3}, RAYWHITE);
        DrawTextEx(this->font, keyToPrint, (Vector2) {(x += button->data.width / 2 - textSizeX), (y += button->data.height / 2 - textSizeY + 3)}, sizeFont, 0, BLACK);
        idSelect = id;
    } else {
        DrawTextureV(button->texture, (Vector2){x, y}, RAYWHITE);
        DrawTextEx(this->font, keyToPrint, (Vector2) {(x += button->data.width / 2 - textSizeX), (y += button->data.height / 2 - textSizeY)}, sizeFont, 0, BLACK);
    }
}

std::string Interface::guiGetString(const char *image, std::string &text, int sizeFont, float x, float y, int sizeStringMax, Color color)
{
    dataAssets *title = this->title[image];
    centerVarToPosition<int, float>(title->data.width, x);
    centerVarToPosition<int, float>(title->data.height, y);
    Vector2 posText = {x, y};
    Rectangle textbox = {x, y, static_cast<float>(title->data.width), static_cast<float>(title->data.height)};
    Vector2 sizeText = MeasureTextEx(this->font, text.c_str(), textbox.height / 2, 0);


    posText.x += textbox.width / 2 - sizeText.x / 2;
    posText.y += sizeText.y / 3;
    if (CheckCollisionPointRec(this->mouse, textbox)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
        int key = GetCharPressed();
        if ((key >= 32) && (key <= 125) && (text.size() < sizeStringMax))
            text.push_back(static_cast<char>(key));
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
            text.pop_back();
        y += 3;
        posText.y += 3;
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
    DrawTexture(title->texture, static_cast<int>(x), static_cast<int>(y), RAYWHITE);
    DrawTextEx(this->font, text.c_str(), posText, textbox.height / 2, 0, RAYWHITE);
    return text;
}

void Interface::displayCircle(Color c, float size, int x, int y)
{
    centerVarToPosition<float, int>(size * 2, x);
    centerVarToPosition<float, int>(size * 2, y);
    DrawCircle(x, y, size, c);
}
