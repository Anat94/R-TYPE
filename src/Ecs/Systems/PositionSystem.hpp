/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** PositionSystem
*/

#ifndef POSITIONSYSTEM_HPP_
    #define POSITIONSYSTEM_HPP_
    #include <chrono>
    #include "Systems.hpp"

class PositionSystem : public ISystems {
    public:
        PositionSystem(): m_start(std::chrono::high_resolution_clock::now()) {};

        float getElapsedTime() const {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float, std::milli> duration = end - m_start;
            return duration.count();
        }

        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel) {
            try {
                if (getElapsedTime() >= 100) {
                    m_start = std::chrono::high_resolution_clock::now();
                    for (auto &&[_, p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
                        if (p.has_value() && v.has_value()) {
                            p->x += v->_dx;
                            p->y += v->_dy;
                        }
                    }
                }
            } catch (std::exception &e) {
                std::cout << e.what();
            }
        };
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif /* !POSITIONSYSTEM_HPP_ */
