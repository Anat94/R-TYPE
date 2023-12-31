/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** ResetOnMoveSystem
*/

#ifndef RESETONMOVESYSTEM_HPP_
#define RESETONMOVESYSTEM_HPP_
#include "Systems.hpp"

class ResetOnMoveSystem : public ISystems {
    public:
        ResetOnMoveSystem() {};

        void operator()(sparse_array<component::Velocity> &vel, sparse_array<component::ResetOnMove> &res) {
            for (auto &&[_, v, r] : zipper<sparse_array<component::Velocity>, sparse_array<component::ResetOnMove>>(vel, res)) {
                if (v.has_value() && r.has_value()) {
                    v->_dx = 0;
                    v->_dy = 0;
                }
            }
        };
    private:
};

#endif /* !RESETONMOVESYSTEM_HPP_ */
