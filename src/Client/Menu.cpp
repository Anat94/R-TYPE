/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.cpp
*/

#include "Menu.hpp"

Menu::Menu()
{
    _window.create(sf::VideoMode(800, 600), "Menu");
    _font.loadFromFile("src/Client/assets/font.ttf");
    _title.setFont(_font);
    _title.setString("NAPTE");
}

Menu::~Menu()
{
}

int Menu::run() {
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    return (0);
        }
        _window.clear();
        _window.draw(_title);

        _window.display();
    }
    return 0;
}