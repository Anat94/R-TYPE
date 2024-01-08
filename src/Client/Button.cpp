/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Button
*/

#include "Button.hpp"

Button::Button(float x, float y, float width, float height, sf::Font& font, std::string text, sf::Color textColor_, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor) : isButtonPressed(false), isButtonHover(false), textColor(textColor_) ,idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor) {
    this->shape.setPosition(sf::Vector2f(x, y));
    this->shape.setSize(sf::Vector2f(width, height));
    this->shape.setFillColor(this->idleColor);

    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(20);
    this->text.setFillColor(textColor_);

    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    this->text.setPosition(sf::Vector2f(x + 60, y + 35));
}

Button::~Button()
{
}

void Button::update(const sf::Vector2f& mousePos)
{
    if (this->shape.getGlobalBounds().contains(mousePos)) {
        this->isButtonHover = true;
        this->shape.setFillColor(this->hoverColor);
    } else {
        this->isButtonHover = false;
        this->shape.setFillColor(this->idleColor);
    }
}

void Button::render(sf::RenderWindow& window) 
{
    window.draw(this->shape);
    window.draw(this->text);
}

bool Button::isPressed() const {
    return this->isButtonPressed;
}

void Button::setState(const bool state) {
    this->isButtonPressed = state;
    if (state) {
        this->shape.setFillColor(this->activeColor);
    } else {
        if (this->isButtonHover)
            this->shape.setFillColor(this->hoverColor);
        else
            this->shape.setFillColor(this->idleColor);
    }
}