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

    sf::Vector2f position(300, 300);

    inputBox.setSize(sf::Vector2f(300, 40));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setPosition(position);

    text.setCharacterSize(10);
    text.setFillColor(sf::Color::Black);
    text.setPosition(position.x + 5, position.y + 5);

    username = "";
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
            handleInput(event);
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
        draw(_window);
        _btnPlay.render(_window);
        _window.display();
    }
    return SUCCES;
}

void Menu::handleInput(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && username.getSize() > 0) {
                username.erase(username.getSize() - 1, 1);
            } else if (event.text.unicode != '\b') {
                username += event.text.unicode;
            }
            text.setString(username);
        }
    }
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(inputBox);
    window.draw(text);
}

std::string Menu::getUsername() const {
    return username.toAnsiString();
}