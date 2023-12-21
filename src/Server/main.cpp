/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"
#include "../Errors.hpp"

using asio::ip::udp;

rtype::event::EventListener listener;

int error_handling(int nb_args)
{
    if (nb_args != 2)
        throw ArgumentError("./server <server_port>");
    return 0;
}


auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, component::DrawableContent& _) {
    for (auto &&[p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
        if (p.has_value() && v.has_value()) {
            p->x += v->_dx;
            p->y += v->_dy;
        }
    }
};

auto reset_on_move_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::ResetOnMove> &res, component::DrawableContent& _) {
    for (auto &&[v, r] : zipper<sparse_array<component::Velocity>, sparse_array<component::ResetOnMove>>(vel, res)) {
        if (v.has_value() && r.has_value()) {
            v->_dx = 0;
            v->_dy = 0;
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
    int first_ent_idx = 0;
    for (auto &&[v, c] : zipper<sparse_array<component::Velocity>, sparse_array<component::Controllable>>(vel, con)) {
        if (c.has_value() && v.has_value()) {
            if (content.event->type == sf::Event::KeyPressed) {
                if (content.event->key.code == sf::Keyboard::Up)
                    v->_dy = -30;
                if (content.event->key.code == sf::Keyboard::Down)
                    v->_dy = 30;
                if (content.event->key.code == sf::Keyboard::Left)
                    v->_dx = -30;
                if (content.event->key.code == sf::Keyboard::Right)
                    v->_dx = 30;
                if (content.event->key.code == sf::Keyboard::Space)
                    listener.addEvent(new rtype::event::ShootEvent(first_ent_idx, -1));
            }
        }
        first_ent_idx++;
    }
};

auto scale_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Scale> &sca, component::DrawableContent& _) {
    for (auto &&[d, s] : zipper<sparse_array<component::Drawable>, sparse_array<component::Scale>>(dra, sca)) {
        if (d.has_value() && s.has_value())
            d->_sprite.setScale(s->_scale.first, s->_scale.second);
    }
};

auto rotation_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Rotation> &rot, component::DrawableContent& _) {
    for (auto &&[d, r] : zipper<sparse_array<component::Drawable>, sparse_array<component::Rotation>>(dra, rot)) {
        if (d.has_value() && r.has_value())
            d->_sprite.setRotation(r->_degrees);
    }
};

auto collision_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& _)
{
    int first_ent_idx = 0;
    for (auto &&[d1, p1] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
        if (!d1.has_value() || !p1.has_value()) continue;
        int second_ent_idx = 0;
        for (auto &&[d2, p2] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
            if (first_ent_idx == second_ent_idx)
                continue;
            if ((p1->x <= p2->x &&
                p1->y <= p2->y &&
                (p1->x + 100) >= p2->x &&
                (p1->y + 100) >= p2->y) ||
                (p2->x <= p1->x &&
                p2->y <= p1->y &&
                (p2->x + 100) >= p1->x &&
                (p2->y + 100) >= p1->y)) {
                    rtype::event::CollisionEvent* new_event = new rtype::event::CollisionEvent(second_ent_idx, first_ent_idx);
                if (listener.hasEvent(new_event)) {
                    second_ent_idx++;
                    delete new_event;
                    continue;
                } else
                    listener.addEvent(new_event);
            } else {

            }
            second_ent_idx++;
        }
        first_ent_idx++;
    }
};

void logging_system(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel) {
    for (auto&& [p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
        std::cout << 0 << ": Position = { " << p.value().x << ", " << p.value().y
            << " }, Velocity = { " << v.value()._dx << ", " << v.value()._dy << " }" << std::endl;
    }
}

void runServer(const char *argv, registry &ecs) {
    asio::io_service service;
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

    ecs.add_system<component::Velocity, component::Controllable>(control_system);
    ecs.add_system<component::Position, component::Velocity>(position_system);
    ecs.add_system<component::Velocity, component::ResetOnMove>(reset_on_move_system);
    ecs.add_system<component::Drawable, component::Position>(collision_system);
    ecs.add_system<component::Drawable, component::Scale>(scale_system);
    ecs.add_system<component::Drawable, component::Rotation>(rotation_system);

    error_handling(argc);
    std::thread serverThread([&]() {
        runServer(argv[1], ecs);
    });
    while (true) {
        component::DrawableContent content = component::DrawableContent(event);
        ecs.run_systems(content);
        while (listener.popEvent());
    }
    return 0;
}