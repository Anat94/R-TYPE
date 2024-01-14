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
    #include "../RoomUtils.hpp"
    #include "../CampaignUtils.hpp"

class EnemyGeneration: public ISystems {
    public:
        /**
         * @brief Construct a new Enemy Generation object
         *
         * @param listener  Listener to add events
         * @param maxEnemiesPerPlayer_  Maximum number of enemies per player
         */
        EnemyGeneration(EventListener *listener, int maxEnemiesPerPlayer_ = 3) : _listener(listener), maxEnemiesPerPlayer(maxEnemiesPerPlayer_) {
            timer.restart();
        };

        /**
         * @brief  operator ()
         *
         * @param pos position of the entity
         * @param hlt health of the entity
         * @param edp endpoint of the entity
         * @param rms List of rooms
         * @param cam List of campaigns mode rooms
         */
        void operator()(sparse_array<component::Position> &pos, sparse_array<component::Health> &hlt, sparse_array<component::Endpoint> &edp, sparse_array<component::Room> &rms, sparse_array<component::CampaignMode> &cam) {
            if (timer.getElapsedTime() > 2500) {
                std::vector<std::string> allRooms = RoomUtils::GetAll(rms);
                std::vector<std::string> allCamps = CampaignUtils::GetAll(cam);
                for (size_t i = 0; i < allRooms.size(); ++i) {
                    if (std::find(allCamps.begin(), allCamps.end(), allRooms[i]) != allCamps.end())
                        continue;
                    int totalEnemies = 0;
                    int totalPlayers = 0;
                    for (auto &&[idx, p, h, e, r] : zipper<sparse_array<component::Position>, sparse_array<component::Health>, sparse_array<component::Endpoint>, sparse_array<component::Room>>(pos, hlt, edp, rms)) {
                        if (r.has_value() && r->_name != allRooms[i])
                            continue;
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
                                {"idle", {true, {0, 7}}}
                            }, allRooms[i]));
                    }
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
