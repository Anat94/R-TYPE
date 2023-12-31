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

class CollisionSystem : public ISystems {
    public:
        CollisionSystem(EventListener *listener) : _listener(listener) {};

        void operator()(sparse_array<component::Hitbox> &dra, sparse_array<component::Position> &pos) {
            for (auto &&[first_ent_idx, h1, p1] : zipper<sparse_array<component::Hitbox>, sparse_array<component::Position>>(dra, pos)) {
                if (!h1.has_value() || !p1.has_value()) continue;
                for (auto &&[second_ent_idx, h2, p2] : zipper<sparse_array<component::Hitbox>, sparse_array<component::Position>>(dra, pos)) {
                    if (first_ent_idx == second_ent_idx || !h2.has_value() || !p2.has_value() || *h1 == *h2)
                        continue;
                    auto temp_h1 = h1->update(*p1);
                    auto temp_h2 = h2->update(*p2);
                    if (temp_h1.isTouching(temp_h2) || temp_h2.isTouching(temp_h1)) {
                        CollisionEvent* new_event = new CollisionEvent(first_ent_idx + 1, second_ent_idx);
                        if (_listener->hasEvent(new_event)) {
                            second_ent_idx++;
                            delete new_event;
                            continue;
                        } else
                            _listener->addEvent(new_event);
                    }
                }
            }
        };
    private:
        EventListener *_listener;
};

#endif /* !COLLISIONSYSTEM_HPP_ */
