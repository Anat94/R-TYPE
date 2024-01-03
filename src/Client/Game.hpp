// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP
//#pragma warning(disable: 4668)
//#pragma warning(disable: 4626)
//#pragma warning(disable: 4625)
//#pragma warning(disable: 4820)
//#pragma warning(disable: 5031)
//#pragma warning(disable: 4365)
//#pragma warning(disable: 5027)
//#pragma warning(disable: 4514)
//#pragma warning(disable: 4464)
//#pragma warning(disable: 5026)
//#pragma warning(disable: 4457)
//#pragma warning(disable: 5262)
//#pragma warning(disable: 5204)
//#pragma warning(disable: 4355)
//#pragma warning(disable: 5220)
//#pragma warning(disable: 5039)
#include "LibLoader.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Game
{
    private:
        sf::RenderWindow _window;
        sf::Event _event;
        sf::Sprite _spriteBackground;
        LibLoader _lib;

    public:
        Game(int sizeX, int sizeY, std::string title);
        ~Game();
        void run();
        void manageEvent();
        void initParalaxBackground();
};

#endif // GAME_HPP
