/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** ButtonSystem
*/

#ifndef BUTTONSYSTEM_HPP_
    #define BUTTONSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Registry.hpp"
    #include "../Events.hpp"

class ButtonSystem : public ISystems {
    public:
        ButtonSystem(sf::Vector2i *mousePos) : _mousePos{mousePos} {};

        void operator()(sparse_array<component::Button> &buttons, sparse_array<component::Position> &positions) {

            for (auto &&[entity, button, position] : zipper<sparse_array<component::Button>, sparse_array<component::Position>>(buttons, positions)) {
                if (!button.has_value() || !position.has_value()) continue;
                //std::cout << "buttonsystem" << std::endl;
                sf::Vector2f buttonPos = sf::Vector2f(position->x, position->y);
                float buttonWidth = button->_width;
                float buttonHeight = button->_height;
                float buttonScale = button->_scale;

                //buttonWidth *= buttonScale;
                //buttonHeight *= buttonScale;

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    std::cout << "buttonWidth: " << buttonWidth << std::endl;
                    std::cout << "buttonHeight: " << buttonHeight << std::endl;
                    std::cout << "mousepos x: " << _mousePos->x << " mousepos y: " << _mousePos->y << std::endl;
                    std::cout << "buttonPos x: " << buttonPos.x << " buttonPos y: " << buttonPos.y << std::endl;
                    
                    if (_mousePos->x >= buttonPos.x && _mousePos->x <= (buttonPos.x + buttonWidth) && _mousePos->y >= buttonPos.y && _mousePos->y <= (buttonPos.y + buttonHeight)) {
                        std::cout << "buttoooooooooon" << std::endl;
                        button->_function();
                    }
                }
            }
        };
    private:
        sf::Vector2i *_mousePos;
        std::unordered_map<entity_t, bool> _hovered;
};

#endif /* !BUTTONSYSTEM_HPP_ */
