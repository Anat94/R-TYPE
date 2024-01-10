/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** InputSystems
*/

#ifndef INPUTSYSTEM_HPP_
    #define INPUTSYSTEM_HPP_
        #include "Systems.hpp"
    #include "../Registry.hpp"
    #include "../Events.hpp"

class InputSystem : public ISystems {
    public:
        InputSystem(sf::Event event, sf::Vector2i *mousePos, sf::RenderWindow *window) : _event(event), _mousePos{mousePos}, _window(window) {};

        void operator()(sparse_array<component::Input> &input, sparse_array<component::Position> &positions, sparse_array<component::Scale> &scale) {

            for (auto &&[entity, input, position, scale] : zipper<sparse_array<component::Input>, sparse_array<component::Position>, sparse_array<component::Scale>>(input, positions, scale)) {
                if (!input.has_value() || !position.has_value()) continue;
                sf::Vector2f inputPos = sf::Vector2f(position->x, position->y);

                float inputWidth = input->_width;
                float inputHeight = input->_height;
                float intputScale = scale->_scale.first;
                
                float new_width_button = inputWidth * intputScale;
                float new_height_button = inputHeight * intputScale;

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //std::cout << "new_height_button" << new_width_button << std::endl;
                    //std::cout << "new_height_button" << new_height_button << std::endl;
                    if (_mousePos->x >= inputPos.x && _mousePos->x <= (inputPos.x + new_width_button) && 
                        _mousePos->y >= inputPos.y && _mousePos->y <= (inputPos.y + new_height_button)) {
                            input->_state = true;
                            std::cout << "dans if pressed" << std::endl;
                    } else {
                        input->_state = false;
                    }
                }
                if (input->_state == true) {
                    while (_window->pollEvent(_event)) {
                        if (_event.type == sf::Event::TextEntered) {
                            if (_event.text.unicode < 128) {
                                if (_event.text.unicode == '\b' && input->_username.size() > 0) {
                                    input->_username.erase(input->_username.size() - 1, 1);
                                } else if (_event.text.unicode != '\b' && input->_username.size() < 14) {
                                    input->_username += _event.text.unicode;
                                }
                                input->_text->setString(input->_username);
                            }
                        }   
                    }
                }
            }
        };

    private:
        sf::Event _event;
        sf::Vector2i *_mousePos;
        sf::RenderWindow *_window;
};

#endif /* !INPUTSYSTEM_HPP_ */
