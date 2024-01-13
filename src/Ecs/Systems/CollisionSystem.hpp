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
    #include "../RoomUtils.hpp"

class CollisionSystem : public ISystems {
    public:
        /**
         * @brief Construct a new Collision System object
         *
         * @param listener  Listener to add events
         */
        CollisionSystem(EventListener *listener) : _listener(listener) {};

        /**
         * @brief operator ()
         *
         * @param dra   List of hitboxes
         * @param pos   List of positions
         * @param rms   List of rooms
         */
        void operator()(sparse_array<component::Hitbox> &dra, sparse_array<component::Position> &pos, sparse_array<component::Room> &rms) {
            std::vector<std::string> allRooms = RoomUtils::GetAll(rms);
            for (size_t i = 0; i < allRooms.size(); ++i) {
                for (auto &&[first_ent_idx, h1, p1, r1] : zipper<sparse_array<component::Hitbox>, sparse_array<component::Position>, sparse_array<component::Room>>(dra, pos, rms)) {
                    if (!h1.has_value() || !p1.has_value() || (r1.has_value() && r1->_name != allRooms[i])) continue;
                    for (auto &&[second_ent_idx, h2, p2, r2] : zipper<sparse_array<component::Hitbox>, sparse_array<component::Position>, sparse_array<component::Room>>(dra, pos, rms)) {
                        if (first_ent_idx == second_ent_idx || !h2.has_value() || !p2.has_value() || (r2.has_value() && r2->_name != allRooms[i]))
                            continue;
                        if (h1->isTouching(*p1, *p2, *h2) || h2->isTouching(*p2, *p1, *h1)) {
                            CollisionEvent* new_event = new CollisionEvent(first_ent_idx, second_ent_idx);
                            if (_listener->hasEvent(new_event))
                                delete new_event;
                            else
                                _listener->addEvent(new_event);
                        }
                    }
                }
            }
        };
    private:
        EventListener *_listener;
};

#endif /* !COLLISIONSYSTEM_HPP_ */
