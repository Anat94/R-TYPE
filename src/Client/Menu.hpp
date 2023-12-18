/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.hpp
*/

#ifndef MENU_HPP_
    #define MENU_HPP_
    #include <SFML/Graphics.hpp>
    #include "Button.hpp"

enum state {
    MENU,
    GAME,
    END,
    ERROR,
    SUCCES,
};

class Menu {
    public:
        Menu();
        ~Menu();
        enum state run();

    private:
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Text _title;
        Button _btnPlay;
};

#endif /* !MENU_HPP_ */