/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** ControlSystem
*/

#ifndef CONTROLSYSTEM_HPP_
    #define CONTROLSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Events.hpp"
    #include <SFML/Graphics.hpp>

class ControlSystem : public ISystems {
    public:
        ControlSystem(EventListener *listener, sf::Event *event) : _listener(listener), _event(event) {};

        void operator()(sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con) {
            std::cout << "Control system with: " << con.size() << std::endl;
            for (auto &&[idx, v, c] : zipper<sparse_array<component::Velocity>, sparse_array<component::Controllable>>(vel, con)) {
                if (c.has_value() && v.has_value()) {
                    if (_event->type == sf::Event::KeyPressed) {
                        if (_event->key.code == sf::Keyboard::Up)
                            v->_dy = -30;
                        if (_event->key.code == sf::Keyboard::Down)
                            v->_dy = 30;
                        if (_event->key.code == sf::Keyboard::Left)
                            v->_dx = -30;
                        if (_event->key.code == sf::Keyboard::Right)
                            v->_dx = 30;
                        if (_event->key.code == sf::Keyboard::Space)
                            _listener->addEvent(new ShootEvent(idx, -1));
                    }
                }
            }
        };
    private:
        EventListener *_listener;
        sf::Event *_event;
};

#endif /* !CONTROLSYSTEM_HPP_ */
