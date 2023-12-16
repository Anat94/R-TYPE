/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** GraphicalEncapsulation
*/

#include "GraphicalEncapsulation.hpp"

rtype::graphical::GraphicalEncapsulation::GraphicalEncapsulation(GraphicalLib lib) : _lib(lib)
{
    if (lib == rtype::graphical::SFML)
        init_SFML();
    else if (lib == rtype::graphical::RAYLIB)
        init_RAYLIB();
}

void rtype::graphical::GraphicalEncapsulation::init_SFML()
{
    _window.create(sf::VideoMode(BASIC_SCREEN_WIDTH, BASIC_SCREEN_HEIGHT), SCREEN_NAME, sf::Style::Close);
    _window.setFramerateLimit(BASIC_FPS);
}

void rtype::graphical::GraphicalEncapsulation::init_RAYLIB()
{
    InitWindow(BASIC_SCREEN_WIDTH, BASIC_SCREEN_HEIGHT, SCREEN_NAME);
    SetTargetFPS(BASIC_FPS);
}

void rtype::graphical::GraphicalEncapsulation::draw(std::any toDraw,  rtype::graphical::components::Position2 pos)
{
    if (_lib == rtype::graphical::GraphicalLib::SFML) {
        auto& elem = *std::any_cast<sf::Sprite*>(toDraw);
        elem.setPosition(pos._x, pos._y);
        _window.draw(elem);
    } else if (_lib == rtype::graphical::GraphicalLib::RAYLIB) {
        Texture2D texture = *std::any_cast<Texture2D*>(toDraw);
        DrawTexture(texture, pos._x, pos._y, WHITE);
    }
}

void rtype::graphical::GraphicalEncapsulation::window_clear(std::any color)
{
    if (_lib == rtype::graphical::GraphicalLib::SFML) {
        auto _color = std::any_cast<sf::Color>(color);
        _window.clear(_color);
    } else if (_lib == rtype::graphical::GraphicalLib::RAYLIB) {
        BeginDrawing();
        auto _color = std::any_cast<Color>(color);
        ClearBackground(_color);
    }
}

void rtype::graphical::GraphicalEncapsulation::window_display()
{
    if (_lib == rtype::graphical::GraphicalLib::SFML)
        _window.display();
    else if (_lib == rtype::graphical::GraphicalLib::RAYLIB)
        EndDrawing();
}

bool rtype::graphical::GraphicalEncapsulation::handleEvent()
{
    if (_lib == rtype::graphical::GraphicalLib::SFML) {
        sf::Event event;
        _window.pollEvent(event);
        if (event.type == sf::Event::Closed)
            return true;
    } else if (_lib == rtype::graphical::GraphicalLib::RAYLIB) {
        if (WindowShouldClose())
            return true;
    }
    return false;
}

rtype::graphical::GraphicalEncapsulation::~GraphicalEncapsulation()
{
    if (_lib == rtype::graphical::GraphicalLib::SFML)
        _window.close();
    else if (_lib == rtype::graphical::GraphicalLib::RAYLIB)
        CloseWindow();
}
