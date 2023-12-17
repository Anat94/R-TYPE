/*
** EPITECH PROJECT, 2023
** Bootstrap
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include "Events.hpp"
#include "ZipperIterator.hpp"

rtype::event::EventListener listener;

auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, component::DrawableContent& _) {
    for (auto &&[p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos, vel)) {
        if (p.has_value() && v.has_value()) {
            p->x += v->_dx;
            p->y += v->_dy;
            if (v->_reset_on_move) {
                v->_dx = 0;
                v->_dy = 0;
            }
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
    for (auto &&[v, c] : zipper<sparse_array<component::Velocity>, sparse_array<component::Controllable>>(vel, con)) {
        if (c.has_value() && v.has_value()) {
            if (content.event->type == sf::Event::KeyPressed) {
                if (content.event->key.code == sf::Keyboard::Up)
                    v->_dy = -5;
                if (content.event->key.code == sf::Keyboard::Down)
                    v->_dy = 5;
                if (content.event->key.code == sf::Keyboard::Left)
                    v->_dx = -5;
                if (content.event->key.code == sf::Keyboard::Right)
                    v->_dx = 5;
            }
        }
    }
};

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& content) {
    for (auto &&[d, p] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(dra, pos)) {
        if (d.has_value() && p.has_value()) {
            d->shape->setPosition(p->x, p->y);
            content.window->draw(*d->shape);
        }
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
            rtype::event::CollisionEvent* new_event = new rtype::event::CollisionEvent(second_ent_idx, first_ent_idx);
            if (listener.hasEvent(new_event)) {
                second_ent_idx++;
                delete new_event;
                continue;
            }
            if (p1.value().x <= p2.value().x &&
                p1.value().y <= p2.value().y &&
                p1.value().x + 100 >= p2.value().x &&
                p1.value().y + 100 >= p2.value().y) {
                listener.addEvent(new_event);
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

int main(int argc, char *argv[]) {
    sf::RenderWindow window;
    sf::Event event;
    registry ecs;

    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();
    ecs.register_component<component::Drawable>();
    ecs.register_component<component::Controllable>();
    ecs.register_component<component::HurtsOnCollision>();
    ecs.register_component<component::Player>();

    entity_t entity1 = ecs.spawn_entity();

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f, true));
    ecs.add_component(entity1, component::Player(100, 20));
    ecs.add_component(entity1, component::Controllable());
    ecs.add_component(entity1, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));

    entity_t entity2 = ecs.spawn_entity();

    ecs.add_component(entity2, component::Position(500.0f, 500.0f));
    ecs.add_component(entity2, component::Velocity(-1.0f, -1.0f));
    ecs.add_component(entity2, component::HurtsOnCollision(10));
    ecs.add_component(entity2, component::Drawable(new sf::CircleShape(50, 50), sf::Color::White));

    window.create(sf::VideoMode(1920, 1080), "Ecs window", sf::Style::Close | sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    listener.addRegistry(ecs);

    ecs.add_system<component::Position, component::Velocity>(position_system);
    ecs.add_system<component::Velocity, component::Controllable>(control_system);
    ecs.add_system<component::Drawable, component::Position>(draw_system);
    ecs.add_system<component::Drawable, component::Position>(collision_system);

    while (true) {
        window.clear();
        window.pollEvent(event);
        component::DrawableContent content = component::DrawableContent(window, event);
        ecs.run_systems(content);
        if (listener.popEvent());
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                break;
        window.display();
    }
    window.close();
    return 0;
}
