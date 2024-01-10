/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
    #define COMPONENTS_HPP_
    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <unordered_map>
    #include <asio.hpp>
    #include "../Errors.hpp"

using asio::ip::udp;
using entity_t = size_t;
/**
 * @brief Used to store the animation and the state of the animation.
 * 
 * (ex: animation["idle"].first for the base sprite index, and animation["idle"].second for the last sprite index)
 * 
 */
using animation_t = std::unordered_map<std::string, std::pair<bool, std::pair<int, int>>>;

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
        Position(float _x, float _y) : x(_x), y(_y) {};
        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
        bool operator==(const Position& other) { return x == other.x && y == other.y; };
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
        /**
         * @brief Position constructor
         * 
         * @param function to execute
         * 
         * @param _width width of the button
         * 
         * @param _height height of the button
         * 
         * @param _scale scale of the button
         * 
        */
        std::function<void()> _function;
        int _width;
        int _height;
        float _scale;
        /**
         * @brief Construct a new Button object
         * 
         * @param function the function to be set
         */
        Button(std::function<void()> function, int width, int height, float scale) : _function(function), _width(width), _height(height), _scale(scale) {}
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
        Pierce(int pierce = 1) : _pierce(pierce) {};
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

    /**
     * @brief Hitbox structure to store the hitbox of an entity
     * 
     */
    struct Hitbox {
        /**
         * @brief the top left position of the hitbox
         * 
         */
        Position _top_left;
        /**
         * @brief the top right position of the hitbox
         * 
         */
        Position _top_right;
        /**
         * @brief the bottom left position of the hitbox
         * 
         */
        Position _bottom_left;
        /**
         * @brief the bottom right position of the hitbox
         * 
         */
        Position _bottom_right;
        /**
         * @brief Construct a new Hitbox object
         * 
         * @param top_left the top left position of the hitbox to be set
         * @param top_right the top right position of the hitbox to be set
         * @param bottom_left the bottom left position of the hitbox to be set
         * @param bottom_right the bottom right position of the hitbox to be set
         */
        Hitbox(const Position &top_left, const Position &top_right, const Position &bottom_left, const Position &bottom_right) :
            _top_left(top_left), _top_right(top_right),
            _bottom_left(bottom_left), _bottom_right(bottom_right)
        {};
        /**
         * @brief Construct a new Hitbox object. Construct 
         * 
         * @param top_left the top left position of the hitbox to be set
         * @param bottom_right the bottom right position of the hitbox to be set
         */
        Hitbox(const Position &top_left, const Position &bottom_right) :
            _top_left(top_left), 
            _top_right(Position(bottom_right.x, top_left.y)),
            _bottom_left(Position(top_left.x, bottom_right.y)),
            _bottom_right(bottom_right)
        {};
        bool isOverlap(float p1, float q1, float p2, float q2) {
            return (p1 < q2) && (q1 > p2);
        }
        bool isTouching(const Hitbox &other) {
            return (_bottom_right.x >= other._top_left.x && _top_left.x <= other._bottom_right.x) &&
                (_bottom_right.y >= other._top_left.y && _top_left.y <= other._bottom_right.y);
        };
        Hitbox update(const Position &pos) {
            Hitbox new_one = *this;
            new_one._top_left.x += pos.x;
            new_one._top_left.y += pos.y;
            new_one._top_right.x += pos.x;
            new_one._top_right.y += pos.y;
            new_one._bottom_left.x += pos.x;
            new_one._bottom_left.y += pos.y;
            new_one._bottom_right.x += pos.x;
            new_one._bottom_right.y += pos.y;
            return new_one;
        };
        bool contains(int x, int y) {
            return (x >= _top_left.x && y >= _top_left.y && x <= _bottom_right.x && y <= _bottom_right.y);
        };
        friend std::ostream& operator<<(std::ostream& os, const Hitbox& hitbox) {
            // os << "Top Left: (" << hitbox._top_left.x << ", " << hitbox._top_left.y << "), ";
            // os << "Top Right: (" << hitbox._top_right.x << ", " << hitbox._top_right.y << "), ";
            // os << "Bottom Left: (" << hitbox._bottom_left.x << ", " << hitbox._bottom_left.y << "), ";
            // os << "Bottom Right: (" << hitbox._bottom_right.x << ", " << hitbox._bottom_right.y << ")";
            return os;
        };
        bool operator==(const Hitbox& other) const {
          return _top_left == other._top_left &&
                 _top_right == other._top_right &&
                 _bottom_left == other._bottom_left &&
                 _bottom_right == other._bottom_right;
        };
    };

    /**
     * @brief Clickable structure to store if an entity is clickable, and if yes its status
     * 
     */
    struct Clickable {
        /**
         * @brief the status of the entity
         * 
         */
        std::string _status;
        /**
         * @brief Construct a new Clickable object
         * 
         * @param status the status to be set
         */
        Clickable(const std::string& status = "idle") : _status(status) {};
    };

    /**
     * @brief AnimatedDrawable structure to animate sprites
     * 
     */
    struct AnimatedDrawable {
        /**
         * @brief path to the spritesheet
         * 
         */
        std::string _path;
        /**
         * @brief set of numbers representing the amount of sprites to animate in the spritesheet
         * 
         * @param[in] _nbSprites A pair of integers specifying the number of sprites in the spritesheet.
         *                      It follows the format {x, y} where:
         *                      - x: number of sprites in a row
         *                      - y: number of rows (not used currently)
         * 
         * (ex: {5, 1} for 5 sprites on 1 line)
         * ! The second argument is not used yet !
         */
        std::pair<int, int> _nbSprites;
        /**
         * @brief set of numbers representing the size of the sprites to load
         * 
         * (ex: {32, 14} for a 32x14 sprite)
         * 
         */
        std::pair<int, int> _spriteSize;
        /**
         * @brief set of numbers representing the size of the gapes between sprites
         * 
         * (ex: {3, 1} for a 3x1 gap)
         * 
         */
        std::pair<int, int> _gaps;
        /**
         * @brief set of numbers representing the size of the offset of the first sprite
         * 
         */
        std::pair<int, int> _firstOffset;
        /**
         * @brief set of numbers representing the indexes of the animation
         * 
         * (ex: {2, 0} to start with the 3rd sprite)
         * ! The second argument is not used yet !
         */
        std::pair<int, int> _currentIdx;
        /**
         * @brief the current animation name
         * 
         * (ex: "idle")
         * 
         */
        std::string _state;
        /**
         * @brief animations of the current AnimatedDrawable object
         * 
         */
        animation_t _anims;
        /**
         * @brief Construct a new Animated Drawable object
         * 
         * @param path path to the spritesheet
         * @param nbSprites number of sprites to load (ex: {10, 1} for ten sprites on 1 line)
         * @param spriteSize size of the sprites in the spritesheet (ex: {34, 12} for a 34 by 12 sprite)
         * @param gaps size of the horizontal and vertical gaps in the spritesheet (ex: {1, 3} for a 1 pixel gap in the x-axis, and a 3 pixel gap in the y-axis)
         * @param firstOffset size of the first offset of the sprite (ex: {101, 4} if the sprite you want start at 101 pixels in the x-axis, and 4 pixels in the y-axis)
         * @param curretnIdx current index of the texture of the sprite (ex: {2, 0} to start at the third sprite of the spritesheet)
         */
        AnimatedDrawable(
            const std::string &path,
            std::pair<int, int> nbSprites,
            std::pair<int, int> spriteSize,
            std::pair<int, int> gaps,
            std::pair<int, int> firstOffset = {0, 0},
            std::pair<int, int> curretnIdx = {0, 0}
        ) :
            _path(path), _nbSprites(nbSprites), _spriteSize(spriteSize), _gaps(gaps), _firstOffset(firstOffset), _currentIdx(curretnIdx), _state("idle") {};
        /**
         * @brief Adds an animation to the animation list
         * 
         * @param state the name of the animation / state of the entity
         * @param indexes the sprites concerned by the animation (sorted for use)
         * @param reset to know if the animation reset when finished or not
         */
        void addAnimation(const std::string &state, std::pair<int, int> indexes, bool reset)
        {
            _anims.emplace(state, std::make_pair(reset, indexes));
        };
    };

    struct Input {
        sf::Text *_text;
        std::string _username;
        float _width;
        float _height;
        bool _state = false;

        Input(sf::Text *text, std::string username, float widht, float height) : _text(text), _username(username), _width(widht), _height(height) {
            _text->setString(text->getString());
        };
    };
};

#endif /* !COMPONENTS_HPP_ */