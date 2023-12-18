/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Registry
*/

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <any>
#include <SFML/Graphics.hpp>
#include "./SparseArray.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../Errors.hpp"

using entity_t = size_t;

namespace component {
    /**
     * @brief Position structure containing x and y coordinates
    */
    struct Position {
        /**
         * @brief x Coordinate value
         */
        float x;
        /**
         * @brief y Coordinate value
         */
        float y;
        /**
         * @brief Position constructor
         * 
         * @param _x X Coordinate value
         * @param _y Y Coordinate value
         * 
        */
        Position(float _x, float _y) : x(_x), y(_y) {}
        bool operator==(const Position& other) { return x == other.x && y == other.y; }
    };

    /**
     * @brief Player structure containing player's info
     */
    struct Player {
        /**
         * @brief Actual level of the player
         */
        int _level;
        /**
         * @brief Health of the player
         */
        int _health;
        /**
         * @brief Number of damage per hit of the player
         */
        int _damage;
        int _xp = 0;
        /**
         * @brief Construct a new Player object
         * 
         * @param health The health of the player
         * @param damage The damage deal by the player
         * @param level The level of the player. Default is 0
         */
        Player(int health, int damage, int level = 0) : _health(health), _damage(damage), _level(level) {};
    };

    /**
     * @brief HurtsOnCollision structure indicating if a collision causes damage
     */
    struct HurtsOnCollision {
        /**
         * @brief Number of damage taken on contact
         */
        int damage;

        entity_t _sender;

        /**
         * @brief Number of pierce of the object
         * 
         */
        int _pierce;
        /**
         * @brief Construct a new Hurts On Collision object
         * 
         * @param _damage The damage of the collision
         */
        HurtsOnCollision(int _damage, entity_t sender = -1, int pierce = 1) : damage(_damage), _sender(sender), _pierce(pierce) {};
    };

    /**
     * @brief Velocity structure containing direction coordinates
    */
    struct Velocity {
        /**
         * @brief direction value on the x-axis
         */
        float _dx;
        /**
         * @brief direction value on the y-axis
         */
        float _dy;
        /**
         * @brief 
         */
        bool _reset_on_move;
        /**
         * @brief Velocity constructor
         * 
         * @param _dx direction value on the x-axis
         * @param _dy direction value on the y-axis
         * 
        */
        Velocity(float dx, float dy, bool reset_on_move = false) : _dx(dx), _dy(dy), _reset_on_move(reset_on_move) {}
    };

    /**
     * @brief Drawable structure for the SFML library
    */
    struct Drawable {
        /**
         * @brief Pointer to an sf::Shape object from the SFML Library
        */
        // sf::Shape *shape;

        sf::Texture _texture;
        sf::Sprite _sprite;
        std::string _path;
        std::pair<float, float> _scale;
        int _rotate = 0;
        /**
         * @brief Drawable constructor
         *
         * @param _shape SFML sf::shape object
         * @param _color SFML Color object representing the color of the shape
         *
        */
        Drawable(const std::string &path_to_texture, std::pair<float, float> scale = {1.0, 1.0}, int rotate = 0) {
            _path = path_to_texture;
            _scale = scale;
            _rotate = rotate;
        };

        void set() {
            if (!_texture.loadFromFile(_path))
                throw SFMLError("Could not load sprite");
            _sprite.setTexture(_texture);
            _sprite.setScale(_scale.first, _scale.second);
            _sprite.setRotation(_rotate);
        };
        // Drawable(const std::string &path_to_texture, sf::IntRect rect, int scale_x = 1, int scale_y = 1) {
        //     if (!_texture.loadFromFile(path_to_texture))
        //         throw std::runtime_error("Could not load sprite");
        //     _sprite.setTexture(_texture);
        //     _sprite.setTextureRect(rect);
        //     _sprite.setScale(scale_x, scale_y);
        // };
    };

    /**
     * @brief Controllable system structure
     * 
    */
    struct Controllable {
        Controllable() {};
    };

    /**
     * @brief DrawableContent util structure
     * 
    */
    struct DrawableContent {
        sf::RenderWindow *window;
        sf::Event *event;
        DrawableContent(sf::RenderWindow &_window, sf::Event &_event) : window(&_window), event(&_event) {};
    };

    /**
     * @brief Heading structure
     *
     */
    struct Heading {
        /**
         * @brief rotation of the heading
         *
         */
        float _rotation;
        /**
         * @brief Heading contructor
         *
         */
        Heading(float rotation = 0) : _rotation(rotation) {};
    };
    struct PlayMusic {
        PlayMusic(std::string path) {
            sf::Music music;
            if (!music.openFromFile(path))
                throw SFMLError("Music not found");
            music.play();
        };
    };
};

/**
 * @brief Registry class managing all the sparse_arrays
 *
*/
class registry {
    public:
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
         * @param f Function representing the system to add to the registry
         * 
        */
        template <typename... Components, typename Function>
        void add_system(Function&& f) {
            auto callable = [f = std::forward<Function>(f)](registry& r, component::DrawableContent& content) {
                f(r.get_components<Components>()..., content);
            };
            systems.push_back(callable);
        }

        /**
         * @brief run all the systems on the registry
         * 
         * @param content DrawableContent structure containing sf::RenderWindow, and sf::Event
         * 
        */
        void run_systems(component::DrawableContent &content) {
            for (auto& system : systems) {
                system(*this, content);
            }
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
        std::vector<std::function<void(registry&, component::DrawableContent&)>> systems;
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
