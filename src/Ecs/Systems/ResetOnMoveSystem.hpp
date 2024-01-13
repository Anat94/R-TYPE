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
        /**
         * @brief Construct a new Reset On Move System object
         * 
         */
        ResetOnMoveSystem() {
            timer.restart();
        };

        /**
         * @brief operator ()
         *
         * @param vel velocity of the entity
         * @param res res of the entity
         */
        void operator()(sparse_array<component::Velocity> &vel, sparse_array<component::ResetOnMove> &res) {
            if (timer.getElapsedTime() > threshold) {
                timer.restart();
                for (auto &&[_, v, r] : zipper<sparse_array<component::Velocity>, sparse_array<component::ResetOnMove>>(vel, res)) {
                    if (v.has_value() && r.has_value()) {
                        v->_dx = 0;
                        v->_dy = 0;
                    }
                }
            }
        };
    private:
        Timer timer;
        int threshold = 100;
};

#endif /* !RESETONMOVESYSTEM_HPP_ */
