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

        void simulateJump(float* y, bool* _jump) {
            std::cout << "jumpe start simulateJump: " << _jump << std::endl;
            for (int i = 0; i < 100; ++i) {
                (*y) += 2;
                std::cout << "Coordonnée y : " << (*y) << std::endl;
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            for (int i = 0; i < 100; ++i) {
                (*y) -= 2;
                std::cout << "Coordonnée y : " << (*y) << std::endl;
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            *_jump = true;
            std::cout << "jumpe end simulateJump: " << _jump << std::endl;
        }

        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Jump> &jump) {
            std::vector<std::thread> threads;
            for (auto &&[entity, p, j] : zipper<sparse_array<component::Position>, sparse_array<component::Jump>>(pos, jump)) {
                if (j.has_value() && p.has_value()) {
                    if (_event->type == sf::Event::KeyPressed) {
                        if (_event->key.code == sf::Keyboard::Space && j->_jump) {
                            
                            std::cout << "jumpe start if: " << j->_jump << std::endl;
                            j->_jump = false;

                            std::thread jumpThread([this, &p, &j]() {
                                this->simulateJump(&p->y, &j->_jump);
                            });

                            jumpThread.join();
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
