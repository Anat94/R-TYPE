/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** CollisionSystem
*/

#ifndef COLLISIONSYSTEM_HPP_
    #define COLLISIONSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Registry.hpp"
    #include "../Events.hpp"

class CollisionSystem {
    public:
        CollisionSystem(EventListener *listener) : _listener(listener) {};

        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos) {
            for (auto &&[first_ent_idx, d1, p1] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
                if (!d1.has_value() || !p1.has_value()) continue;
                for (auto &&[second_ent_idx, d2, p2] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
                    if (first_ent_idx == second_ent_idx)
                        continue;
                    if ((p1->x <= p2->x &&
                        p1->y <= p2->y &&
                        (p1->x + 100) >= p2->x &&
                        (p1->y + 100) >= p2->y) ||
                        (p2->x <= p1->x &&
                        p2->y <= p1->y &&
                        (p2->x + 100) >= p1->x &&
                        (p2->y + 100) >= p1->y)) {
                            CollisionEvent* new_event = new CollisionEvent(second_ent_idx, first_ent_idx);
                        if (_listener->hasEvent(new_event)) {
                            second_ent_idx++;
                            delete new_event;
                            continue;
                        } else
                            _listener->addEvent(new_event);
                    } else {

                    }
                }
            }
        };
    private:
        EventListener *_listener;
};

#endif /* !COLLISIONSYSTEM_HPP_ */
