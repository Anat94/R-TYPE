/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/
//#pragma warning(disable: 4668)
//#pragma warning(disable: 4626)
//#pragma warning(disable: 4625)
//#pragma warning(disable: 4820)
//#pragma warning(disable: 5031)
//#pragma warning(disable: 4365)
//#pragma warning(disable: 5027)
//#pragma warning(disable: 4514)
//#pragma warning(disable: 4464)
//#pragma warning(disable: 5026)
//#pragma warning(disable: 4457)
//#pragma warning(disable: 5262)
//#pragma warning(disable: 5204)
//#pragma warning(disable: 4355)
//#pragma warning(disable: 5220)
//#pragma warning(disable: 5039)
#include <iostream>
#include <asio.hpp>
#include "Server.hpp"
#include "../Errors.hpp"
#include "../Ecs/Systems/KillWhenOutOfBounds.hpp"
#include "../Ecs/Systems/EnemyGeneration.hpp"
#include "../Ecs/Systems/ShieldSystem.hpp"
#include "../Ecs/Systems/KillOnTimerSystem.hpp"
#include "../Ecs/Systems/AutomaticShootSystem.hpp"

using asio::ip::udp;

EventListener listener;

/**
 * @brief error handling
 *
 * @param nb_args number of arguments
 * @return int 0 if success
 */
int error_handling(int nb_args)
{
    if (nb_args != 2)
        throw ArgumentError("./server <server_port>");
    return 0;
}

/**
 * @brief main function
 *
 * @param argc number of arguments
 * @param argv arguments given
 * @return int 0 if success
 * @return int 84 if failure
 */
int main(int argc, char *argv[]) {
    std::mutex mtx;
    sf::Event event;
    registry ecs;
    ecs.mtx = &mtx;

    error_handling(argc);

    ecs.register_component<component::Scale>();
    ecs.register_component<component::Score>();
    ecs.register_component<component::Damage>();
    ecs.register_component<component::Health>();
    ecs.register_component<component::Hitbox>();
    ecs.register_component<component::Pierce>();
    ecs.register_component<component::Heading>();
    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();
    ecs.register_component<component::Drawable>();
    ecs.register_component<component::Rotation>();
    ecs.register_component<component::PlayMusic>();
    ecs.register_component<component::ResetOnMove>();
    ecs.register_component<component::ServerEntity>();
    ecs.register_component<component::Controllable>();
    ecs.register_component<component::HurtsOnCollision>();
    ecs.register_component<component::AnimatedDrawable>();
    ecs.register_component<component::Endpoint>();
    ecs.register_component<component::Room>();
    ecs.register_component<component::Username>();
    ecs.register_component<component::Host>();
    ecs.register_component<component::Shield>();
    ecs.register_component<component::KillOnTimer>();
    ecs.register_component<component::ShootCounter>();
    ecs.register_component<component::AutoShoot>();
    entity_t decoy = ecs.spawn_entity();
    ecs.add_component<component::Room>(decoy, component::Room("__"));

    listener.addRegistry(ecs);

    PositionSystem *pos_sys = new PositionSystem();
    ecs.add_system<component::Position, component::Velocity>(*pos_sys);
    KillWhenOutOfBounds *kill_sys = new KillWhenOutOfBounds(&listener, {1920, 1080});
    ecs.add_system<component::Position, component::Velocity>(*kill_sys);
    EnemyGeneration *engen_sys = new EnemyGeneration(&listener, 2);
    ecs.add_system<component::Position, component::Health, component::Endpoint, component::Room>(*engen_sys);
    CollisionSystem *col_sys = new CollisionSystem(&listener);
    ecs.add_system<component::Hitbox, component::Position, component::Room, component::Shield>(*col_sys);
    ShieldSystem *shd_sys = new ShieldSystem(&listener);
    ecs.add_system<component::Shield>(*shd_sys);
    KillOnTimerSystem *kot_sys = new KillOnTimerSystem(&listener);
    ecs.add_system<component::KillOnTimer>(*kot_sys);
    AutomaticShootSystem *ash_sys = new AutomaticShootSystem(&listener, 1500);
    ecs.add_system<component::AutoShoot>(*ash_sys);
    asio::io_context service;
    Server *server = new Server(service, std::atoi(argv[1]), ecs, listener, mtx);
    service.run();
    ecs.add_system<component::AnimatedDrawable, component::Scale, component::Position, component::Endpoint>(*server);

    while (true) {
        mtx.lock();
        ecs.run_systems();
        while (listener.popEvent());
        mtx.unlock();
    }
    return 0;
}