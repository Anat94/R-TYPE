/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Menu.hpp
*/

#ifndef MENU_HPP_
    #define MENU_HPP_
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
    #include <SFML/Graphics.hpp>
    #include "Button.hpp"

enum state {
    MENU = 5,
    GAME = 1,
    END = 2,
    ERRORS = 0,
    SUCCES = 4
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
        sf::Text _highScore;
        Button _btnPlay;
        sf::RectangleShape inputBox;
        sf::Text text;
        sf::String username;
        int lastScore = 0;
        std::string lastUsername;
};

#endif /* !MENU_HPP_ */