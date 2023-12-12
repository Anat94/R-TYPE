/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** TestZipperIterator
*/

#include <SFML/Graphics.hpp>
#include <criterion/criterion.h>
#include "../Registry.hpp"
#include "../ZipperIterator.hpp"

Test(works_in_for, global_test) {
    registry ecs;
    
    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();

    entity_t entity1 = ecs.spawn_entity();
    component::Position pos(10.0f, 10.0f);
    component::Velocity vel(0.0f, 0.0f);

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f));

    for (auto&& [p, v] : zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(ecs.get_components<component::Position>(), ecs.get_components<component::Velocity>())) {
        cr_assert_eq(p.value().x, pos.x);
        cr_assert_eq(p.value().y, pos.y);
        cr_assert_eq(v.value().dx, vel.dx);
        cr_assert_eq(v.value().dy, vel.dy);
    }
}

Test(operator_plus_plus, error_index_too_big) {
    registry ecs;
    bool error = false;
    
    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();

    entity_t entity1 = ecs.spawn_entity();
    component::Position pos(10.0f, 10.0f);
    component::Velocity vel(0.0f, 0.0f);

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f));
    zipper<sparse_array<component::Position>, sparse_array<component::Velocity>> zip(ecs.get_components<component::Position>(), ecs.get_components<component::Velocity>());
    auto it = zip.begin();
    ++it;
    try {
        ++it;
        ++it;
    } catch (std::exception) {
        error = true;
    }
    cr_assert_eq(error, true);
}
