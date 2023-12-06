/*
** EPITECH PROJECT, 2023
** Bootstrap
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include "Registry.hpp"

namespace component {
    struct Position {
        float x, y;
        Position(float _x, float _y) : x(_x), y(_y) {}
    };

    struct Velocity {
        float dx, dy;
        Velocity(float _dx, float _dy) : dx(_dx), dy(_dy) {}
    };

    struct Drawable {
        sf::Shape *shape;
        Drawable(sf::Shape *_shape, sf::Color _color) : shape(_shape) {
            shape->setFillColor(_color);
        }
    };

    struct Controllable {
        void move() {
            pos.x += 10;
            pos.y += 10;
        }
        Position pos;
        Controllable(Position _pos) : pos(_pos) {}
    };
};

auto position_system = [](sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel, sf::RenderWindow& _) {
    for (size_t i = 0; i < pos.size() && i < vel.size(); ++i) {
        if (pos[i] && vel[i]) {
            pos[i]->x += vel[i]->dx;
            pos[i]->y += vel[i]->dy;
        }
    }
};

auto control_system = [](sparse_array<component::Velocity> &vel, sparse_array<component::Controllable> &con, sf::RenderWindow& _) {
    for (size_t i = 0; i < con.size() && i < vel.size(); ++i) {
        if (con[i]) {
            vel[i]->dx += 1;
            vel[i]->dy += 1;
        }
    }
};

auto draw_system = [](sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, sf::RenderWindow& window) {
    for (size_t i = 0; i < dra.size() && i < pos.size(); ++i) {
        if (dra[i] && pos[i]) {
            std::cout << "Drawing at position: {"
                      << pos[i]->x << ", " << pos[i]->y << "}" << std::endl;
            dra[i].value().shape->setPosition(pos[i]->x, pos[i]->y);
            window.draw(*dra[i].value().shape);
        }
    }
};

void logging_system(sparse_array<component::Position> &pos, sparse_array<component::Velocity> &vel) {
    for (size_t i = 0; i < pos.size() && i < vel.size(); ++i) {
        if (pos[i] && vel[i]) {
            std::cerr << i << " : Position = { " << pos[i]->x << " , " << pos[i]->y
                      << " } , Velocity = { " << vel[i]->dx << " , " << vel[i]->dy
                      << " } " << std::endl;
        }
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
    ecs.add_component(entity1, component::Velocity(5.0f, 5.0f));
    ecs.add_component(entity1, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Blue));

    entity_t entity2 = ecs.spawn_entity();

    ecs.add_component(entity2, component::Position(10.0f, 10.0f));
    ecs.add_component(entity2, component::Drawable(new sf::RectangleShape({100, 100}), sf::Color::Green));

    entity_t entity3 = ecs.spawn_entity();

    ecs.add_component(entity3, component::Position(10.0f, 10.0f));
    ecs.add_component(entity3, component::Drawable(new sf::CircleShape(100, 100), sf::Color::Green));

    window.create(sf::VideoMode(1920, 1080), "Bootstrap", sf::Style::Close | sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    ecs.add_system<component::Position, component::Velocity>(position_system);
    ecs.add_system<component::Velocity, component::Controllable>(control_system);
    ecs.add_system<component::Drawable, component::Position>(draw_system);
    logging_system(ecs.get_components<component::Position>(), ecs.get_components<component::Velocity>());
    while (true) {
        window.clear();
        window.pollEvent(event);
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::Escape)
                break;

        ecs.run_systems(window);
        window.display();
    }
    window.close();
    return 0;
}
