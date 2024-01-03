/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** ScaleSystem
*/

#ifndef SCALESYSTEM_HPP_
#define SCALESYSTEM_HPP_
#include "Systems.hpp"

class ScaleSystem : public ISystems {
    public:
        ScaleSystem() {};

        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Scale> &sca) {
            std::cout << "Scale system" << std::endl;
            for (auto &&[_, d, s] : zipper<sparse_array<component::Drawable>, sparse_array<component::Scale>>(dra, sca)) {
                if (d.has_value() && s.has_value())
                    // d->_sprite.setScale(s->_scale.first, s->_scale.second);
                    continue;
            }
        };
};

#endif /* !SCALESYSTEM_HPP_ */
