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
        void handleInput(sf::Event& event);
        void draw(sf::RenderWindow& window);
        std::string getUsername() const;

    private:
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Text _title;
        Button _btnPlay;
        sf::RectangleShape inputBox;
        sf::Text text;
        sf::String username;
};

#endif /* !MENU_HPP_ */