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
#include <iostream>
#include "LibLoader.hpp"
#include "Game.hpp"

Game::Game(int sizeX, int sizeY, std::string title): _lib("./libecs.so")
{
    this->_window.create(sf::VideoMode(sizeX, sizeY), title);
    initParalaxBackground();
}

Game::~Game()
{
    if (this->_window.isOpen())
        this->_window.close();
}

void Game::initParalaxBackground() {
    sf::Texture texture;
    if (!texture.loadFromFile("./assets/background.png")) {
        std::cout << "Error loading background" << std::endl;
    }
    _spriteBackground.scale(2, 2);
    _spriteBackground.setTexture(texture);
}


void Game::manageEvent()
{
    while (this->_window.pollEvent(_event))
    {
        if (_event.type == sf::Event::Closed)
            this->_window.close();
        if (_event.type == sf::Event::KeyPressed) {
            if (_event.key.code == sf::Keyboard::Escape)
                this->_window.close();
            if (_event.key.code == sf::Keyboard::Up)
                std::cout << "Up" << std::endl;
            if (_event.key.code == sf::Keyboard::Down)
                std::cout << "Down" << std::endl;
            if (_event.key.code == sf::Keyboard::Left)
                std::cout << "Left" << std::endl;
            if (_event.key.code == sf::Keyboard::Right)
                std::cout << "Right" << std::endl;
        }
    }
}

void Game::run()
{
    while (this->_window.isOpen()) {
        manageEvent();
        this->_window.clear();
        this->_window.draw(_spriteBackground);
        this->_window.display();
    }
}