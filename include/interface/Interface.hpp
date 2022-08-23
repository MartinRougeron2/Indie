/*
** EPITECH PROJECT, 2021
** indiestudio
** File description:
** Interface
*/

#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <raylib.h>
#include <string>
#include <map>

#define sizeFontTitle 18
#define sizeFontSubtitle 16
#define sizeFontText 13

struct dataAssets {
    Texture2D texture;
    Image data;
    Rectangle sourceRec;
};

class Interface {
    public:
        Interface(void);
        ~Interface();

        /*!
        ** @brief set size screen, setting variables usefull to others function
        */
        void setSizeScreen(void);

        /*!
        ** @brief set size screen, setting variables usefull to others function
        */
        void setMusic(int state, bool mute, int volume);

        /*!
        ** @brief set assets like button, fonts, ..
        */
        void setAssets(void);
        dataAssets *getAssets(const char *a);

        /*!
        ** @brief get mouse position
        */
        Vector2 getMouse() const;

        /*!
        ** @brief print a button at x-y position
        ** @param image image name
        ** @param pourcent value to scale the button to the screen size
        ** @param pos image position
        ** @param select display selected image
        ** @return click or not on button
        */
        bool displayButton(const char *image, float pourcent, float x, float y, bool select = 0);

        /*!
        ** @brief print an image at x-y position
        ** @param image image name
        ** @param pourcent value to scale the button to the screen size
        ** @param pos image position
        */
        void displayImage(const char *image, float pourcent, float x, float y);

        /*!
        ** @brief print an image at x-y position
        ** @param pourcent value to scale the button to the screen size
        ** @param pos image position
        ** @param key alias to user input
        ** @param selected alias to select
        */
        void interactifText(int id, float pourcent, float x, float y, int &key, int &idDelect);

        /*!
        ** @brief display text
        ** @param text text to print
        ** @param sizeFont text size
        ** @param x x position
        ** @param y y position
        */
        void displayText(const char *text, int sizeFont, int x, int y, Color c = BLACK);

        /*!
        ** @brief display text
        ** @param c color
        ** @param size circle size
        ** @param x x position
        ** @param y y position
        */
        void displayCircle(Color c, float size, int x, int y);

        /*!
        ** explosion sound
        ** @param name sound to play
        */
        void playSound(const char *name);

        /*!
        ** @brief display a slide bar between startValue and endValue, return selected value
        ** @param size bar size
        ** @param actualValue size bar in slide bar
        ** @param startValue start value, default 0
        ** @param endValue end value, default 100
        ** @return got selected value
        */
        int guiSlideBar(Rectangle size, float actualValue, Vector2 mouse, float endValue = 100);

        /*!
        ** @brief display text
        ** @param image background to display
        ** @param text text to print
        ** @param x x position
        ** @param y y position
        ** @param sizeStringMax size max to the text
        ** @param color color text
        ** @return string get by uid
        */
        std::string guiGetString(const char *image, std::string &text, int sizeFont, float x, float y, int sizeStringMax, Color color = BLACK);

        // width screen
        int sizeX;

        // height screen
        int sizeY;
    protected:
    private:
        std::map<std::string, dataAssets *> button;
        std::map<std::string, dataAssets *> title;
        std::map<std::string, Music> sound;
        Font font;
        Vector2 mouse;
};

template <typename T, typename P>
inline void centerVarToPosition(T width, P &x)
{
    x -= (static_cast<P>(width) / 2);
}

#endif /* !INTERFACE_HPP_ */
