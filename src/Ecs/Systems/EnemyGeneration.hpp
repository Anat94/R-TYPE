/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** EnemyGeneration
*/

#ifndef ENEMYGENERATION_HPP_
    #define ENEMYGENERATION_HPP_
    #include "../Events.hpp"
    #include "../ZipperIterator.hpp"
    #include "../../Random.hpp"
    #include "../../Timer.hpp"

class EnemyGeneration: public ISystems {
    public:
        EnemyGeneration(EventListener *listener, int maxEnemiesPerPlayer_ = 3) : _listener(listener), maxEnemiesPerPlayer(maxEnemiesPerPlayer_) {
            timer.restart();
        };

        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Health> &hlt, sparse_array<component::Endpoint> &edp) {
            if (timer.getElapsedTime() > 1500) {
                int totalEnemies = 0;
                int totalPlayers = 0;
                for (auto &&[idx, p, h, e] : zipper<sparse_array<component::Position>, sparse_array<component::Health>, sparse_array<component::Endpoint>>(pos, hlt, edp)) {
                    if (p.has_value() && h.has_value() && !e.has_value()) {
                        totalEnemies++;
                    } else if (e.has_value()) {
                        totalPlayers++;
                    }
                };
                if ((maxEnemiesPerPlayer * totalPlayers) > totalEnemies) {
                    _listener->addEvent(new SpawnEnemy(
                        200,
                        6.0f,
                        component::Velocity(-12.0f, 0.0f),
                        component::Position(1920, Random::generate(0, 920)),
                        component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet5.gif", {7, 0}, {21, 24}, {12, 0}, {5, 5}),
                        {
                            {"idle", {{0, 7}, true}}
                        }));
                }
                timer.restart();
            }
        };

    private:
        int maxEnemiesPerPlayer = 3;
        EventListener *_listener;
        Timer timer;
};

#endif /* !ENEMYGENERATION_HPP_ */