/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** DrawSystem
*/

#ifndef DRAWSYSTEM_HPP_
#define DRAWSYSTEM_HPP_
#include "Systems.hpp"

class DrawSystem : public ISystems {
    public:
        DrawSystem(sf::RenderWindow *window) : _window(window) {};
        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos) {
            for (auto &&[idx, d, p] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
                if (d.has_value() && p.has_value()) {
                    d->_sprite.setPosition(p->x, p->y);
                    _window->draw(d->_sprite);
                }
            }
        };
    private:
        sf::RenderWindow *_window;
};

#endif /* !DRAWSYSTEM_HPP_ */
