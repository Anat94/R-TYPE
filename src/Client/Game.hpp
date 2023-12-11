// Game.hpp

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Game
{
    private:
        sf::RenderWindow _window;
        sf::Event _event;
        void* _lib;
        sf::Sprite _spriteBackground;


    public:
        Game(int sizeX, int sizeY, std::string title);
        ~Game();
        void run();
        void manageEvent();
        void initParalaxBackground();
};

#endif // GAME_HPP
