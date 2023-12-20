/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Registry
*/

#include "./SparseArray.hpp"
#include "../Errors.hpp"
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <iostream>
#include <any>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

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
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & x;
            ar & y;
        }
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
     * @brief Position structure containing x and y coordinates
    */
    struct ServerEntity {
        /**
         * @brief x Coordinate value
         */
        entity_t entity;
        /**
         * @brief Position constructor
         * 
         * @param _x X Coordinate value
         * @param _y Y Coordinate value
         * 
        */
        ServerEntity(entity_t _entity) : entity(_entity) {}
    };

    /**
     * @brief Button structure containing its state
     * 
     */
    struct Button {

    };

    /**
     * @brief Damage structure to store the attack value of an entity
     * 
     */
    struct Damage {
        /**
         * @brief Number representing the damage value
         * 
         */
        int _damage;
        /**
         * @brief Construct a new Damage object
         * 
         * @param damage the damage to be set
         */
        Damage(int damage) : _damage(damage) {};
    };

    /**
     * @brief Health structure for the player's health
     * 
     */
    struct Health {
        /**
         * @brief Number representing the health
         * 
         */
        int _health;
        /**
         * @brief Construct a new Health object
         * 
         * @param health the health to be set
         */
        Health(int health) : _health(health) {};
    };

    /**
     * @brief Score structure for the player's score
     * 
     */
    struct Score {
        /**
         * @brief Number representing the score
         * 
         */
        int _score;
        /**
         * @brief Construct a new Score object
         * 
         * @param score the score to be set
         */
        Score(int score = 0) : _score(score) {};
    };

    /**
     * @brief ResetOnMove structure to know if the velocity of an entity shouldbe reset after moving
     * 
     */
    struct ResetOnMove {
        /**
         * @brief Construct a new ResetOnMove object
         * 
         */
        ResetOnMove() {};
    };

    /**
     * @brief Pierce structure representing the pierce of an entity. Mostly used for projectile
     * 
     */
    struct Pierce {
        /**
         * @brief Number equivalent to the number of times the object can pass through
         * 
         */
        int _pierce;
        /**
         * @brief Construct a new Pierce object
         * 
         * @param pierce The pierce number
         */
        Pierce(int pierce) : _pierce(pierce) {};
    };

    /**
     * @brief HurtsOnCollision structure indicating if a collision causes damage
     */
    struct HurtsOnCollision {
        /**
         * @brief 
         * 
         */
        entity_t _sender;
        /**
         * @brief Construct a new HurtsOnCollision object
         * 
         * @param sender the sender of the object. Default is 0
         */
        HurtsOnCollision(entity_t sender = -1) : _sender(sender) {};
    };

    /**
     * @brief Rotation structure containing the rotation that mostly should be applied to a `component::Drawable`
     * 
     */
    struct Rotation {
        /**
         * @brief Angle of the rotation
         * 
         */
        float _degrees;
        /**
         * @brief Construct a new Rotation object
         * 
         * @param degrees the degrees to be set
         */
        Rotation(float degrees = 0) : _degrees(degrees) {};
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
         * @brief Velocity constructor
         * 
         * @param _dx direction value on the x-axis
         * @param _dy direction value on the y-axis
         * 
        */
        Velocity(float dx, float dy) : _dx(dx), _dy(dy) {}
    };

    /**
     * @brief Drawable structure for the SFML library
    */
    struct Drawable {
        /**
         * @brief Texture of the sprite
        */
        sf::Texture _texture;
        /**
         * @brief Sprite to be printed on screen
         * 
         */
        sf::Sprite _sprite;
        /**
         * @brief path to the image file
         * 
         */
        std::string _path;
        /**
         * @brief Construct a new Drawable object
         * 
         * @param path_to_texture the path to the texture to be set
         */
        Drawable(const std::string &path_to_texture) {
            _path = path_to_texture;
        };
        /**
         * @brief set the texture to the sprite
         * 
         */
        void set() {
            if (!_texture.loadFromFile(_path))
                throw SFMLError("Could not load sprite");
            _sprite.setTexture(_texture);
        };
    };

    /**
     * @brief Scale structure to store the scale of an entity, use it with `component::Drawable`
     * 
     */
    struct Scale {
        /**
         * @brief Pair of float representing the scale of the object
         * 
         */
        std::pair<float, float> _scale;
        /**
         * @brief Construct a new Scale object
         * 
         * @param scale the scale to be set. This constructor will duplicate the value passed as parameter. Default is 1
         */
        Scale(float scale = 1) : _scale({scale, scale}) {};
        /**
         * @brief Construct a new Scale object
         * 
         * @param scale_x the x value of the scale
         * @param scale_y the y value of the scale
         */
        Scale(float scale_x, float scale_y) : _scale({scale_x, scale_y}) {};
        /**
         * @brief Construct a new Scale object
         * 
         * @param scale the scale to be set
         */
        Scale(std::pair<float, float> scale) : _scale({scale.first, scale.second}) {};
    };

    /**
     * @brief Controllable system structure
     * 
    */
    struct Controllable {
        Controllable() {};
    };

    /**
     * @brief endpoint system structure
     * 
     */
    struct Endpoint {
        /**
         * @brief endpoint associated to the entity
         * 
         */
        udp::endpoint _endpoint;
        /**
         * @brief endpoint constructor
         * 
         */
        Endpoint(udp::endpoint endpoint) : _endpoint(endpoint) {};
        bool operator==(const udp::endpoint other) { return _endpoint == other; }
    };

    /**
     * @brief DrawableContent util structure
     * 
    */
    struct DrawableContent {
        sf::RenderWindow *window;
        sf::Event *event;
        DrawableContent(sf::Event &_event) : window(nullptr), event(&_event) {};
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
