/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ParallaxSystem
*/

#ifndef PARALLAXSYSTEM_HPP_
#define PARALLAXSYSTEM_HPP_
    #include "Systems.hpp"

class ParallaxSystem : public ISystems {
    public:
        ParallaxSystem() {};

        void operator()(sparse_array<component::Parallax> &par, sparse_array<component::Position> &pos)
        {
            float elapsedTick = _clock.getElapsedTime().asMilliseconds();
            for (auto &&[idx, d, p] : zipper<sparse_array<component::Parallax>, sparse_array<component::Position>>(par, pos)) {
                if (d.has_value() && p.has_value() && elapsedTick >= _threshold) {
                    p->x -= 5 + (10 * d->_index);
                    _clock.restart();
                }
            }
        };
    private:
        sf::Clock _clock;
        float _threshold = 50.0f;
};

#endif /* !PARALLAXSYSTEM_HPP_ */
