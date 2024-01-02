/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Button
*/

#ifndef BUTTON_HPP_
    #define BUTTON_HPP_
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
    #include <SFML/Graphics.hpp>

class Button {
    public:
        Button(float x, float y, float width, float height, sf::Font& font, std::string text, sf::Color textColor, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
        ~Button();
        void update(const sf::Vector2f& mousePos);
        void render(sf::RenderWindow& window);
        bool isPressed() const;
        void setState(const bool state);
        

    private:
        sf::RectangleShape shape;
        sf::Text text;
        sf::Color textColor;
        sf::Color idleColor;
        sf::Color hoverColor;
        sf::Color activeColor;

        bool isButtonPressed;
        bool isButtonHover;
};

#endif /* !BUTTON_HPP_ */
