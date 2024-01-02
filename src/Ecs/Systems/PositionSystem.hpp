/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** PositionSystem
*/

#ifndef POSITIONSYSTEM_HPP_
#define POSITIONSYSTEM_HPP_
#include "Systems.hpp"

class PositionSystem : public ISystems {
    public:
        PositionSystem() {};

        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel) {
            for (auto &&[_, p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
                if (p.has_value() && v.has_value()) {
                    p->x += v->_dx;
                    p->y += v->_dy;
                }
            }
        };
};

#endif /* !POSITIONSYSTEM_HPP_ */
