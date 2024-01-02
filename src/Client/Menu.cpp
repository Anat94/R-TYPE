/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.cpp
*/
#pragma warning(disable: 4668)
#pragma warning(disable: 4626)
#pragma warning(disable: 4625)
#pragma warning(disable: 4820)
#pragma warning(disable: 5031)
#pragma warning(disable: 4365)
#pragma warning(disable: 5027)
#pragma warning(disable: 4514)
#pragma warning(disable: 4464)
#pragma warning(disable: 5026)
#pragma warning(disable: 4457)
#pragma warning(disable: 5262)
#pragma warning(disable: 5204)
#pragma warning(disable: 4355)
#pragma warning(disable: 5220)
#pragma warning(disable: 5039)
#include "Menu.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Menu::Menu() : _btnPlay(300, 400, 200, 50, _font, "Playing", sf::Color::Black, sf::Color::White, sf::Color::Red, sf::Color::Blue)
{
    _window.create(sf::VideoMode(800, 600), "Menu");
    _font.loadFromFile("src/Client/assets/font.ttf");

    std::ifstream myFile("db.txt");
    if (!myFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing.");
    }
    std::string firstLine;
    if (std::getline(myFile, firstLine)) {
        std::cout << firstLine << std::endl;
        size_t colonPos = firstLine.find(':');
        std::string score = firstLine.substr(colonPos + 1);
        lastUsername = firstLine.substr(0, colonPos);
        lastUsername.erase(lastUsername.find_last_not_of(" ") + 1);
        score.erase(0, score.find_first_not_of(" "));
        score.erase(score.find_last_not_of(" ") + 1);
        lastScore = std::stoi(score);
    } else {
        throw std::runtime_error("Failed to read the first line from the file.");
    }

    _title.setFont(_font);
    _title.setPosition(280, 150);
    _title.setString("NAPTE");
    _title.setCharacterSize(70);

    _highScore.setFont(_font);
    _highScore.setPosition(275, 50);
    _highScore.setCharacterSize(20);
    _highScore.setString("Highscore by " + lastUsername + ": " +  std::to_string(lastScore));

    sf::Vector2f position(300, 300);

    inputBox.setSize(sf::Vector2f(200, 40));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setPosition(position);

    text.setFont(_font);
    text.setCharacterSize(20);
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
                    if (_btnPlay.isPressed() && username.getSize() > 0) {
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
        _window.draw(_highScore);
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
    return username;
}