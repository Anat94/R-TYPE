/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** main
*/

#include <iostream>
#include <fstream>
#include "GraphicalEncapsulation.hpp"

using graph = rtype::graphical::GraphicalEncapsulation;
using state = rtype::graphical::GraphicalLib;

int main(int argc, char *argv[]) {
    graph g(state::RAYLIB);
    Texture2D texture = LoadTexture(argv[1]);
    // sf::Texture _t;
    // _t.loadFromFile(argv[1]);
    // sf::Sprite _s;
    // _s.setTexture(_t);

    while (1) {
        g.window_clear(WHITE);
        if (g.handleEvent())
            break;
        g.draw(&texture, rtype::graphical::components::Position2(300, 400));
        g.window_display();
    }
    // UnloadTexture(texture);
    return 0;
}