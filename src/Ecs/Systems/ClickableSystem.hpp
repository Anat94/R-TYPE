/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClickableSystem
*/

#ifndef CLICKABLESYSTEM_HPP_
    #define CLICKABLESYSTEM_HPP_
    #include "Systems.hpp"

class ClickableSystem : public ISystems {
    public:
        ClickableSystem(sf::Vector2i *mousePos) : _mousePos(mousePos) {};

        void operator()(sparse_array<component::Clickable> &cli, sparse_array<component::Hitbox> &hit) {
            for (auto &&[index, c, h] : zipper<sparse_array<component::Clickable>, sparse_array<component::Hitbox>>(cli, hit)) {
                if (c.has_value() && h.has_value()) {
                    continue;
                }
            }
        }
    private:
        sf::Vector2i *_mousePos;
};

#endif /* !CLICKABLESYSTEM_HPP_ */
