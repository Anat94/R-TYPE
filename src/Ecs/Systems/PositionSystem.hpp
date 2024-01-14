/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** PositionSystem
*/

#ifndef POSITIONSYSTEM_HPP_
    #define POSITIONSYSTEM_HPP_
    #include <chrono>
    #include "Systems.hpp"
    #include "../../Timer.hpp"

class PositionSystem : public ISystems {
    public:
    /**
     * @brief Construct a new Position System object
     *
     */
        PositionSystem(EventListener *listener) : _listener(listener) {
            timer.restart();
        };

        /**
         * @brief operator ()
         *
         * @param pos   List of positions
         * @param vel   List of velocities
         */
        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, sparse_array<component::SurvivesOutOfBound> &sur) {
            try {
                if (timer.getElapsedTime() >= 100) {
                    timer.restart();
                    for (auto &&[idx, p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
                        if (p.has_value() && v.has_value()) {
                            p->x += v->_dx;
                            p->y += v->_dy;
                        }
                        if (sur[idx].has_value() && p.has_value()) {
                            if (p->x < 1920 && p->y < 1080)
                                _listener->addEvent(new RemoveSurvivesEvent(idx));
                        }
                    }
                }
            } catch (std::exception &e) {
                std::cout << e.what();
            }
        };
    private:
        Timer timer;
        EventListener *_listener;
};

#endif /* !POSITIONSYSTEM_HPP_ */
