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

using asio::ip::udp;

EventListener listener;

int error_handling(int nb_args)
{
    if (nb_args != 2)
        throw ArgumentError("./server <server_port>");
    return 0;
}

void logging_system(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel) {
    for (auto&& [_, p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
        std::cout << 0 << ": Position = { " << p.value().x << ", " << p.value().y
            << " }, Velocity = { " << v.value()._dx << ", " << v.value()._dy << " }" << std::endl;
    }
}

void runServer(const char *argv, registry &ecs) {
    asio::io_context service;
    Server server(service, std::atoi(argv), ecs, listener);
    service.run();
}

int main(int argc, char *argv[]) {
    // sf::RenderWindow window;
    sf::Event event;
    registry ecs;
    // sf::Texture _texture;
    // _texture.loadFromFile(argv[1]);

    // sf::Sprite _sprite;
    // _sprite.setTexture(_texture);
    // _sprite.setPosition(100, 100);

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

    // entity_t entity1 = ecs.spawn_entity();

    // ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    // ecs.add_component(entity1, component::Velocity(0.0f, 0.0f, true));
    // ecs.add_component(entity1, component::Player(100, 20));
    // ecs.add_component(entity1, component::Controllable());
    // ecs.add_component(entity1, component::Heading());
    // ecs.add_component(entity1, component::Drawable("../../temp/assets/textures/sprites/Hobbit-Idle1.png"));

    // entity_t entity2 = ecs.spawn_entity();

    // ecs.add_component(entity2, component::Position(700.0f, 500.0f));
    // ecs.add_component(entity2, component::Velocity(0.0f, 0.0f));
    // // ecs.add_component(entity2, component::HurtsOnCollision(10));
    // ecs.add_component(entity2, component::Player(300, 30));
    // ecs.add_component(entity2, component::Drawable("../../temp/assets/textures/sprites/Hobbit-Idle1.png"));

    listener.addRegistry(ecs);

    // ecs.add_system<component::Velocity, component::Controllable>(control_system);
    // ecs.add_system<component::Position, component::Velocity>(position_system);
    PositionSystem *pos_sys = new PositionSystem();
    ecs.add_system<component::Position, component::Velocity>(*pos_sys);
    // ecs.add_system<component::Velocity, component::ResetOnMove>(reset_on_move_system);
    ResetOnMoveSystem *rom_sys = new ResetOnMoveSystem();
    ecs.add_system<component::Velocity, component::ResetOnMove>(*rom_sys);
    // ecs.add_system<component::Drawable, component::Position>(collision_system);
    CollisionSystem *col_sys = new CollisionSystem(&listener);
    ecs.add_system<component::Hitbox, component::Position>(*col_sys);
    // ecs.add_system<component::Drawable, component::Scale>(scale_system);
    // ecs.add_system<component::Drawable, component::Rotation>(rotation_system);

    error_handling(argc);
    std::thread serverThread([&]() {
        runServer(argv[1], ecs);
    });
    while (true) {
        ecs.run_systems();
        while (listener.popEvent());
    }
    serverThread.joinable();
    serverThread.join();
    return 0;
}