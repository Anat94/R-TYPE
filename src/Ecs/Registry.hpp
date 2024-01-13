/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Registry
*/

#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_
    #include <unordered_map>
    #include <typeindex>
    #include <functional>
    #include <typeinfo>
    #include <iostream>
    #include <mutex>
    #include <any>
    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <asio.hpp>
    #include "SparseArray.hpp"
    #include "Components.hpp"
    #include "../Errors.hpp"
    #include "Systems/Systems.hpp"

using asio::ip::udp;

using entity_t = size_t;

/**
 * @brief Registry class managing all the sparse_arrays
 *
*/
class registry {
    public:
        /**
         * @brief mutex used to avoid concurrent thread issues
        */
        std::mutex *mtx;
        // vv TO DELETE vv
        bool can_run_updates = true;
        /**
         * @brief Register a component to the registry, creates an sparse_array for the corresponding component passed in template
         *
         * @returns sparse array reference of the created component in the registry
        */
        template <class Component>
        sparse_array<Component>& register_component() {
            auto typeIdx = std::type_index(typeid(Component));
            if (_components_arrays.find(typeIdx) == _components_arrays.end())
                _components_arrays[typeIdx] = sparse_array<Component>();
            return std::any_cast<sparse_array<Component>&>(_components_arrays[typeIdx]);
        }

        /**
         * @brief get a sparse_array of Component passed as template, from the registry
         *
         * @returns sparse array reference of component in the registry
        */
        template <class Component>
        sparse_array<Component>& get_components() {
            auto typeIdx = std::type_index(typeid(Component));
            return std::any_cast<sparse_array<Component>&>(_components_arrays[typeIdx]);
        }

        /**
         * @brief get a constant sparse_array of Component passed as template, from the registry
         * 
         * @returns sparse array reference of component in the registry
        */
        template <class Component>
        const sparse_array<Component>& get_components() const {
            auto typeIdx = std::type_index(typeid(Component));
            auto it = _components_arrays.find(typeIdx);
            if (it != _components_arrays.end())
                return std::any_cast<const sparse_array<Component>&>(it->second);
            else
                throw std::runtime_error("Component not found in registry");
        }

        /**
         * @brief spawns an entity
         * 
         * @returns returns an entity_id availble for assigning to a new entity
        */
        entity_t spawn_entity() {
            entity_t newEntity = _next_entity_id;
            ++_next_entity_id;
            return newEntity;
        }

        /**
         * @brief Checks if the entity_t was removed from the list of entities
         * 
         * @param ent the entity to check
         * @return true if the entity wasn't removed,
         * @return false otherwise
         */
        bool entity_exists(entity_t ent) {
            auto it = std::find(_dead_entities.begin(), _dead_entities.end(), ent);
            if (it == _dead_entities.end())
                return true;
            return false;
        }

        /**
         * @brief from a size_t index, get an entity_t value
         * 
         * @param idx index to convert
         * 
         * @returns index as size_t
        */
        entity_t entity_from_index(std::size_t idx) {
            return idx;
        }

        /**
         * @brief kill entity, adds the entity to the dead_entities registry
         * 
         * @param e entity to kill
         * 
        */
        void kill_entity(entity_t const &e) {
            _dead_entities.push_back(e);
        }

        /**
         * @brief add a component to the registry in associated sparse_array
         * 
         * @param to entity to add the component to
         * @param c Component to add
         * 
         * @returns sparse array reference of templated component in the registry 
        */
        template <typename Component>
        typename sparse_array<Component>::reference_type add_component(entity_t const &to, Component &&c) {
            auto& components = register_component<Component>();
            return components.insert_at(to, std::forward<Component>(c));
        }

        /**
         * @brief emplace multiple component to the registry in associated sparse_array
         * 
         * @param to entity to add the component to
         * @param p Components to add
         * 
         * @returns sparse array reference of templated component in the registry 
        */
        template <typename Component, typename... Params>
        typename sparse_array<Component>::reference_type emplace_component(entity_t const &to, Params &&...p) {
            auto& components = register_component<Component>();
            return components.emplace_at(to, std::forward<Params>(p)...);
        }

        /**
         * @brief remove templated component from entity passed as parameter
         * 
         * @param from entity to remove component from
         * 
        */
        template <typename Component>
        void remove_component(entity_t const &from) {
            auto& components = get_components<Component>();
            components.erase(from);
        }

        /**
         * @brief add a system to the registry
         * 
         * @param system ISystems representing the system to add to the registry
         * 
        */
        template <typename... Components, typename SystemType>
        void add_system(SystemType&& system) {
            static_assert(std::is_base_of<ISystems, std::decay_t<SystemType>>::value,
                  "SystemType must inherit from ISystems");
            struct Wrapper : public ISystems {
                SystemType system;
                Wrapper(SystemType&& sys) : system(std::forward<SystemType>(sys)) {}
                void operator()(registry& r) {
                    system(r.get_components<Components>()...);
                }
            };
            systems.push_back(Wrapper(std::forward<SystemType>(system)));
        }

        /**
         * @brief run all the systems on the registry
         * 
         * @param content DrawableContent structure containing sf::RenderWindow, and sf::Event
         * 
        */
        void run_systems() {
            // while (!can_run_updates) continue;
            // can_run_updates = false;
            // std::cout << "handling systems\n";
            for (auto& system : systems) {
                system(*this);
            }
            // std::cout << "finished handling systems\n";
            // can_run_updates = true;
        }

    private:
        /**
         * @brief map of sparse arrays for each entity
         * 
         */
        std::unordered_map<std::type_index, std::any> _components_arrays;
        /**
         * @brief systems of the registry
         * 
         */
        std::vector<std::function<void(registry&)>> systems;
        /**
         * @brief list of dead entities
         * 
         */
        std::vector<entity_t> _dead_entities;
        /**
         * @brief next available entity ID
        */
        entity_t _next_entity_id = 0;
};

#endif /* !REGISTRY_HPP_ */
