/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** AutomaticShootSystem
*/

#ifndef AUTOMATICSHOOTSYSTEM_HPP_
    #define AUTOMATICSHOOTSYSTEM_HPP_
    #include "../Events.hpp"
    #include "../ZipperIterator.hpp"
    #include "../../Random.hpp"
    #include "../../Timer.hpp"

class AutomaticShootSystem: public ISystems {
    public:
        /**
         * @brief Construct a new AutomaticShootSystem object
         *
         * @param listener  Listener to add events
         * @param maxEnemiesPerPlayer_  Maximum number of enemies per player
         */
        AutomaticShootSystem(EventListener *listener, int interval = 1500) : _listener(listener), _interval(interval) {
            timer.restart();
        };

        /**
         * @brief  operator ()
         *
         * @param pos position of the entity
         * @param hlt health of the entity
         * @param edp endpoint of the entity
         * @param rms List of rooms
         */
        void operator()(sparse_array<component::AutoShoot> &ash) {
            if (timer.getElapsedTime() > _interval) {
                for (size_t i = 0; i < ash.size(); ++i) {
                    if (ash[i].has_value()) {
                        EnemyShootEvent *evt = new EnemyShootEvent(i);
                        if (_listener->hasEvent(evt))
                            delete evt;
                        else
                            _listener->addEvent(evt);
                    }
                }
                timer.restart();
            }
        };

    private:
        EventListener *_listener;
        Timer timer;
        int _interval = 1500;
};

#endif /* !AUTOMATICSHOOTSYSTEM_HPP_ */
