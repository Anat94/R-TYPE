/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ShieldSystem
*/

#ifndef KILLONTIMER_HPP_
    #define KILLONTIMER_HPP_
    #include "Systems.hpp"
    #include "../Events.hpp"


/**
 * @brief KillOnTimerSystem system, handles the kill of entities after a certain time
 * 
 */
class KillOnTimerSystem : public ISystems {
    public:
        /**
         * @brief Construct a new KillOnTimerSystem System object
         * 
         * @param listener event listener
         */
        KillOnTimerSystem(EventListener *listener) : _listener(listener) {};

        /**
         * @brief system handler checking if the entities are to be killed
         * 
         * @param kot all the killOnTimer components
         */
        void operator()(sparse_array<component::KillOnTimer> &kot) {
            if (timer.getElapsedTime() > 100) {
                for (size_t i = 0; i < kot.size(); ++i) {
                    if (kot[i].has_value() && (kot[i]->timer.getElapsedTime() > kot[i]->deathTime)) {
                        _listener->addEvent(new DeathEvent(i, 0));
                    }
                }
                timer.restart();
            }
        };
    private:
        /**
         * @brief event listener
         * 
         */
        EventListener *_listener;
        /**
         * @brief timer
         * 
         */
        Timer timer;
};

#endif /* !KILLONTIMER_HPP_ */
