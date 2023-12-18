/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.hpp
*/

#include <SFML/Graphics.hpp>

class Menu {
    public:
        Menu();
        ~Menu();
        int run();
    private:
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Text _title;
};