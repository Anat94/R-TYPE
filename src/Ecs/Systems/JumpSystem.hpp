/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** JumpSystem
*/

#ifndef JUMPSYSTEM_HPP_
    #define JUMPSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Registry.hpp"
    #include "../Events.hpp"
    #include "../../Timer.hpp"
    #include <thread>

class JumpSystem : public ISystems {
    public:
        JumpSystem(sf::Event *event) : _event(event) {};

        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Jump> &jump) {
            std::vector<std::thread> threads;
            for (auto &&[entity, p, j] : zipper<sparse_array<component::Position>, sparse_array<component::Jump>>(pos, jump)) {
                if (j.has_value() && p.has_value()) {
                    if (_event->type == sf::Event::KeyPressed) {
                        if (_event->key.code == sf::Keyboard::Space && j->_jump) {
                            
                            std::cout << "jumpe start if: " << j->_jump << std::endl;
                            j->_jump = false;

                            std::cout << "jumpe end if: " << j->_jump << std::endl;
                        }
                    }
                }
            }
        };

    private:
        sf::Event *_event;
};

#endif /* !JUMPSYSTEM_HPP_ */
