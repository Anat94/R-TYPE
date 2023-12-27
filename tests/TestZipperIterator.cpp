#include <gtest/gtest.h>
#include <SFML/Graphics.hpp>
#include "../src/Ecs/Registry.hpp"
#include "../src/Ecs/ZipperIterator.hpp"

TEST(TestZipperIterator, works_in_for_global_test) {
    registry ecs;

    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();

    entity_t entity1 = ecs.spawn_entity();
    component::Position pos(10.0f, 10.0f);
    component::Velocity vel(0.0f, 0.0f);

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f));

    auto pos_components = ecs.get_components<component::Position>();
    auto vel_components = ecs.get_components<component::Velocity>();

    auto it = zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos_components, vel_components).begin();

    for (; it != zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos_components, vel_components).end(); ++it) {
        auto [_, p, v] = *it;
        EXPECT_EQ(p.value().x, pos.x);
        EXPECT_EQ(p.value().y, pos.y);
        EXPECT_EQ(v.value()._dx, vel._dx);
        EXPECT_EQ(v.value()._dy, vel._dy);
    }
}

TEST(TestZipperIterator, operator_plus_plus_error_index_too_big) {
    registry ecs;
    bool error = false;

    ecs.register_component<component::Position>();
    ecs.register_component<component::Velocity>();

    entity_t entity1 = ecs.spawn_entity();
    component::Position pos(10.0f, 10.0f);
    component::Velocity vel(0.0f, 0.0f);

    ecs.add_component(entity1, component::Position(10.0f, 10.0f));
    ecs.add_component(entity1, component::Velocity(0.0f, 0.0f));

    auto pos_components = ecs.get_components<component::Position>();
    auto vel_components = ecs.get_components<component::Velocity>();

    auto it = zipper<sparse_array<component::Position>, sparse_array<component::Velocity>>(pos_components, vel_components).begin();

    try {
        ++it;
        ++it;
        ++it;
    } catch (const std::exception &) {
        error = true;
    }
    EXPECT_EQ(error, true);
}
