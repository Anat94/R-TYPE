#include <unordered_map>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <any>
#include "./SparseArray.hpp"

using entity_t = size_t;

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
        Controllable() {};
    };

    struct DrawableContent {
        sf::RenderWindow *window;
        sf::Event *event;
        DrawableContent(sf::RenderWindow &_window, sf::Event &_event) : window(&_window), event(&_event) {};
    };
};

class registry {
    public:
        template <class Component>
        sparse_array<Component>& register_component() {
            auto typeIdx = std::type_index(typeid(Component));
            if (_components_arrays.find(typeIdx) == _components_arrays.end())
                _components_arrays[typeIdx] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component>&>(_components_arrays[typeIdx]);
        }

        template <class Component>
        sparse_array<Component>& get_components() {
            auto typeIdx = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component>&>(_components_arrays[typeIdx]);
        }

        template <class Component>
        const sparse_array<Component>& get_components() const {
            auto typeIdx = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeIdx);
            if (it != _components_arrays.end())
                return std::any_cast<const sparse_array<Component>&>(it->second);
            else
                throw std::runtime_error("Component not found in registry");
        }

        entity_t spawn_entity() {
            entity_t newEntity = _next_entity_id;
            ++_next_entity_id;
            return newEntity;
        }

        entity_t entity_from_index(std::size_t idx) {
            return idx;
        }

        void kill_entity(entity_t const &e) {
            _dead_entities.push_back(e);
        }

        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &to, Component &&c) {
            auto& components = register_component<Component>();
            return components.insert_at(to, std::forward<Component>(c));
        }

        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &to, Params &&...p) {
            auto& components = register_component<Component>();
            return components.emplace_at(to, std::forward<Params>(p)...);
        }

        template <typename Component>
        void remove_component(entity_t const &from) {
            auto& components = get_components<Component>();
            components.erase(from);
        }

        template <typename... Components, typename Function>
        void add_system(Function&& f) {
            auto callable = [f = std::forward<Function>(f)](registry& r, component::DrawableContent& content) {
                f(r.get_components<Components>()..., content);
            };
            systems.push_back(callable);
        }

        void run_systems(component::DrawableContent &content) {
            for (auto& system : systems) {
                system(*this, content);
            }
        }

    private:
        std::unordered_map<std::type_index, std::any> _components_arrays;
        std::vector<std::function<void(registry&, component::DrawableContent&)>> systems;
        std::vector<entity_t> _dead_entities;
        entity_t _next_entity_id = 0;
};
