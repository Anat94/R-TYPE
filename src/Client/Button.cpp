/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** Button
*/

#include "Button.hpp"

/**
 * @brief Construct a new Button:: Button object
 *
 * @param x position x of the button
 * @param y position y of the button
 * @param width width of the button
 * @param height height of the button
 * @param font font of the text field
 * @param text text of the button
 * @param textColor_ text color of the button
 * @param idleColor idle color of the button
 * @param hoverColor hover color of the button
 * @param activeColor active color of the button
 */
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

/**
 * @brief Destroy the Button:: Button object
 *
 */
Button::~Button()
{
}

/**
 * @brief update the button on hover
 *
 * @param mousePos position of the mouse on the window
 */
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

/**
 * @brief render the button on the window
 *
 * @param window the windows to render
 */
void Button::render(sf::RenderWindow& window)
{
    window.draw(this->shape);
    window.draw(this->text);
}

/**
 * @brief Is the button pressed ?
 *
 * @return true Yes is pressed
 * @return false No is not pressed
 */
bool Button::isPressed() const {
    return this->isButtonPressed;
}

/**
 * @brief set the state of the button
 *
 * @param state state to set
 */
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