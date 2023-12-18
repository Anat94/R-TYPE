/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Button
*/

#ifndef BUTTON_HPP_
    #define BUTTON_HPP_
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
