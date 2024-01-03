/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** RotationSystem
*/

#ifndef ROTATIONSYSTEM_HPP_
#define ROTATIONSYSTEM_HPP_
#include "Systems.hpp"

class RotationSystem : public ISystems {
    public:
        RotationSystem() {};

        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Rotation> &rot) {
            std::cout << "Rotation system" << std::endl;
            for (auto &&[_, d, r] : zipper<sparse_array<component::Drawable>, sparse_array<component::Rotation>>(dra, rot)) {
                if (d.has_value() && r.has_value())
                    d->_sprite.setRotation(r->_degrees);
            }
        };
    private:
};

#endif /* !ROTATIONSYSTEM_HPP_ */
