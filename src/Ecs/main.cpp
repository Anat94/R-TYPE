/*
** EPITECH PROJECT, 2023
** Bootstrap
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include "Registry.hpp"
#include "ZipperIterator.hpp"

auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, component::DrawableContent& _) {
    for (size_t i = 0; i < pos.size() && i < vel.size(); ++i) {
        if (pos[i] && vel[i]) {
            pos[i]->x += vel[i]->_dx;
            pos[i]->y += vel[i]->_dy;
            if (vel[i]->_reset_on_move) {
                vel[i]->_dx = 0;
                vel[i]->_dy = 0;
            }
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, component::DrawableContent& content) {
    for (size_t i = 0; i < con.size() && i < vel.size(); ++i) {
        if (con[i]) {
            if (content.event->type == sf::Event::KeyPressed) {
                if (content.event->key.code == sf::Keyboard::Up)
                    vel[i]->_dy = -5;
                if (content.event->key.code == sf::Keyboard::Down)
                    vel[i]->_dy = 5;
                if (content.event->key.code == sf::Keyboard::Left)
                    vel[i]->_dx = -5;
                if (content.event->key.code == sf::Keyboard::Right)
                    vel[i]->_dx = 5;
            }
        }
    }
};

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, component::DrawableContent& content) {
    for (size_t i = 0; i < dra.size() && i < pos.size(); ++i) {
        if (dra[i] && pos[i]) {
            dra[i].value().shape->setPosition(pos[i]->x, pos[i]->y);
            content.window->draw(*dra[i].value().shape);
        }
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

    entity_t entity1 = ecs.spawn_entity();

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f, true));
    ecs.add_component(entity1, component::Controllable());
    ecs.add_component(entity1, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));

    entity_t entity2 = ecs.spawn_entity();

    ecs.add_component(entity2, component::Position(100.0f, 100.0f));
    ecs.add_component(entity2, component::Velocity(10.0f, 10.0f));
    ecs.add_component(entity2, component::Controllable());
    ecs.add_component(entity2, component::Drawable(new sf::CircleShape(100, 100), sf::Color::White));

    window.create(sf::VideoMode(1920, 1080), "Ecs window", sf::Style::Close | sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    ecs.add_system<component::Position, component::Velocity>(position_system);
    ecs.add_system<component::Velocity, component::Controllable>(control_system);
    ecs.add_system<component::Drawable, component::Position>(draw_system);

    while (true) {
        window.clear();
        window.pollEvent(event);
        component::DrawableContent content = component::DrawableContent(window, event);
        ecs.run_systems(content);
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                break;

        window.display();
    }
    window.close();
    return 0;
}
