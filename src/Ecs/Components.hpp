/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
    #define COMPONENTS_HPP_
    #include <SFML/Graphics.hpp>
    #include <boost/asio.hpp>
    #include <SFML/Audio.hpp>
    #include "../Errors.hpp"

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

#endif /* !COMPONENTS_HPP_ */