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
        /**
         * @brief Construct a new Button System object
         *
         * @param listener Listener to add events
         * @param window Window to get mouse position
         */
        ButtonSystem(EventListener *listener, sf::RenderWindow &window) : _listener(listener), _window(window) {};

        /**
         * @brief operator ()
         *
         * @param buttons List of buttons
         * @param positions List of positions
         */
        void operator()(sparse_array<component::Button> &buttons, sparse_array<component::Position> &positions) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(_window);

            for (auto &&[entity, button, position] : zipper<sparse_array<component::Button>, sparse_array<component::Position>>(buttons, positions)) {
                if (!button.has_value() || !position.has_value()) continue;

                sf::Vector2f buttonPos = sf::Vector2f(position->x, position->y);
                sf::Vector2f buttonSize = sf::Vector2f(button->width, button->height);

                sf::FloatRect buttonBounds(buttonPos, buttonSize);

                bool isMouseHover = buttonBounds.contains(static_cast<sf::Vector2f>(mousePosition));


                // ajouter dans event.hpp hoverEvent et CLickEvent
                if (isMouseHover && !_hovered[entity]) {
                    _hovered[entity] = true;
                    //HoverEvent *hoverEvent = new HoverEvent(entity);
                    //_listener->addEvent(hoverEvent);
                } else if (!isMouseHover && _hovered[entity]) {
                    _hovered[entity] = false;
                    //UnhoverEvent *unhoverEvent = new UnhoverEvent(entity);
                    //_listener->addEvent(unhoverEvent);
                }

                if (isMouseHover && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //ClickEvent *clickEvent = new ClickEvent(entity);
                    //_listener->addEvent(clickEvent);
                }
            }
        };
    private:
        EventListener *_listener;
        sf::RenderWindow &_window;
        std::unordered_map<entity_t, bool> _hovered;
};

#endif /* !BUTTONSYSTEM_HPP_ */
