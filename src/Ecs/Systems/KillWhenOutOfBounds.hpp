/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** KillWhenOutOfBounds
*/

#ifndef KILLWHENOUTOFBOUNDS_HPP_
    #define KILLWHENOUTOFBOUNDS_HPP_
    #include "Systems.hpp"
    #include "../Registry.hpp"
    #include "../Events.hpp"
    #include "../../Timer.hpp"

class KillWhenOutOfBounds: public ISystems {
    public:
        KillWhenOutOfBounds(EventListener *listener, component::Position bounds) : _listener(listener), _bounds(bounds) {
            timer.restart();
        };

        void operator()(sparse_array<component::Position> &pos,sparse_array<component::Velocity> &vel) {
            if (timer.getElapsedTime() > 250) {
                for (auto &&[idx, v, p] : zipper<sparse_array<component::Velocity>, sparse_array<component::Position>>(vel, pos)) {
                    if (!v.has_value() || !p.has_value()) continue;
                    if (p->x < -threshold || p->x > (_bounds.x + threshold) || p->y < -threshold ||  p->y > (_bounds.y + threshold)) {
                        _listener->addEvent(new DeathEvent(idx, 0));
                    }
                }
                timer.restart();
            }
        };
    private:
        Timer timer;
        EventListener *_listener;
        component::Position _bounds;
        float threshold = 200;
};

#endif /* !KILLWHENOUTOFBOUNDS_HPP_ */
