/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** ShieldSystem
*/

#ifndef SHIELDSYSTEM_HPP_
    #define SHIELDSYSTEM_HPP_
    #include "Systems.hpp"
    #include "../Events.hpp"


/**
 * @brief Shield system, handles the shield removal at the end of their duration
 * 
 */
class ShieldSystem : public ISystems {
    public:
        /**
         * @brief Construct a new Shield System object
         * 
         * @param listener event listener
         */
        ShieldSystem(EventListener *listener) : _listener(listener) {};

        /**
         * @brief system handler checking if the shields are past their duration
         * 
         * @param shd all the shields
         */
        void operator()(sparse_array<component::Shield> &shd) {
            if (timer.getElapsedTime() > 100) {
                for (size_t i = 0; i < shd.size(); ++i) {
                    if (shd[i].has_value() && (shd[i]->timer.getElapsedTime() > shd[i]->totalTime)) {
                        _listener->addEvent(new RemoveShieldEvent(i));
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

#endif /* !SHIELDSYSTEM_HPP_ */
