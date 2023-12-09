#include <unordered_map>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <any>
#include "./SparseArray.hpp"

using entity_t = size_t;

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
            auto callable = [f = std::forward<Function>(f)](registry& r, sf::RenderWindow& window) {
                f(r.get_components<Components>()..., window);
            };
            systems.push_back(callable);
        }

        void run_systems(sf::RenderWindow& window) {
            for (auto& system : systems) {
                system(*this, window);
            }
        }

    private:
        std::unordered_map<std::type_index, std::any> _components_arrays;
        std::vector<std::function<void(registry&, sf::RenderWindow&)>> systems;
        std::vector<entity_t> _dead_entities;
        entity_t _next_entity_id = 0;
};
