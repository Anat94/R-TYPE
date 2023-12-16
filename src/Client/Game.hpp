// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP

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
