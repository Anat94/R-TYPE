/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.cpp
*/

#include "Menu.hpp"
#include <iostream>

Menu::Menu() : _btnPlay(300, 400, 200, 50, _font, "Playing", sf::Color::Black, sf::Color::White, sf::Color::Red, sf::Color::Blue)
{
    _window.create(sf::VideoMode(800, 600), "Menu");
    _font.loadFromFile("src/Client/assets/font.ttf");
    _title.setFont(_font);
    _title.setPosition(280, 150);
    _title.setString("NAPTE");
    _title.setCharacterSize(70);
}

Menu::~Menu()
{
}

enum state Menu::run() {
    while (_window.isOpen()) {
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window.close();
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    return SUCCES;

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (_btnPlay.isPressed()) {
                        return GAME;
                    }
                }
            }
        }

        sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
        _btnPlay.update(mousePos);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (_btnPlay.isPressed() == false) {
                _btnPlay.setState(true);
            }
        } else {
            if (_btnPlay.isPressed()) {
                _btnPlay.setState(false);
            }
        }
        _window.clear();
        _window.draw(_title);
        _btnPlay.render(_window);
        _window.display();
    }
    return SUCCES;
}