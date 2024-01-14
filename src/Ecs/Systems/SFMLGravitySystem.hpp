/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** GravitySystem
*/

#ifndef GRAVITYSYSTEM_HPP_
    #define GRAVITYSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Events.hpp"
    #include <SFML/Graphics.hpp>
    #include "../RoomUtils.hpp"

class SFMLGravitySystem : public ISystems{
    public:
        SFMLGravitySystem(EventListener *listener) : _listener(listener), timer() {};

        void operator()(sparse_array<component::Gravity> &gra, sparse_array<component::Hitbox> &hit, sparse_array<component::Position> &pos) {
            for (auto &&[first_ent_idx, g, h, p] : zipper<sparse_array<component::Gravity>, sparse_array<component::Hitbox>, sparse_array<component::Position>(gra, hit, pos)) {
                if (timer.getElapsedTime() > 100) {
                    p->y -= 10;
                    timer.restart();
                }
            }
        };

    private:
        EventListener *_listener;
        sf::Event *_event;
        Timer timer;
};

#endif /* !GRAVITYSYSTEM_HPP_ */


 gravity hitbow position

timer (sfclock) -> temp passe en l'aire

while -> 