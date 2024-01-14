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
    #include "../Timer.hpp"

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
        // sf::RectangleShape shape;
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
        sf::Font font = sf::Font();
        std::string strText = "";
        sf::Color textColor = sf::Color();
        sf::Color idleColor = sf::Color();
        sf::Color hoverColor = sf::Color();
        sf::Color activeColor = sf::Color();
        sf::Text text = sf::Text();
        
        Button(float x_, float y_, float width_, float height_, sf::Font& font_, std::string text_, sf::Color textColor_, sf::Color idleColor_, sf::Color hoverColor_, sf::Color activeColor_):
            x(x_), y(y_), width(width_), height(height_), font(font_), strText(text_), textColor(textColor_), idleColor(idleColor_), hoverColor(hoverColor_), activeColor(activeColor_)
        {}
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
     * @brief Shield component
     * 
     */
    struct Shield {
        /**
         * @brief timer to time the shield's effect
         * 
         */
        Timer timer;
        /**
         * @brief sheild duration
         * 
         */
        int totalTime = 3000;
        /**
         * @brief construct a Shield element
         * 
         * @param time sheild duration
         * 
         */
        Shield(int time = 3000): totalTime(time), timer() {};
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
     * @brief Room structure for the player's room
     * 
     */
    struct Room {
        /**
         * @brief room name
         * 
         */
        std::string _name;
        /**
         * @brief Construct a new Room Object
         * 
         * @param name name of the room
         */
        Room(std::string name) : _name(name) {};
    };

    /**
     * @brief user's name
     * 
    */
    struct Username {
        /**
         * @brief user's name
         * 
         */
        std::string _name;
        /**
         * @brief Construct a new Username Object
         * 
         * @param name name of the room
         */
        Username(std::string name) : _name(name) {};
    };

    /**
     * @brief Host Component name
     * 
    */
    struct Host {
        /**
         * @brief Construct a new Username Object
         * 
         */
        Host() {};
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
     * @brief KillOnTimer Component
     * 
     */
    struct KillOnTimer {
        /**
         * @brief timer to time the entity's life
         * 
         */
        Timer timer;
        /**
         * @brief life duration
         * 
         */
        int deathTime = 3000;

        /**
         * @brief Constructor for the KillOnTimer Component
         * 
         * @param time duration which to keep entity alive
         */
        KillOnTimer(int time): deathTime(time) {};
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
        /**
         * @brief SFML render window
         * 
         */
        sf::RenderWindow *window;
        /**
         * @brief SFML event
         * 
         */
        sf::Event *event;
        /**
         * @brief Drawable Constructor, initialize the window at null
         * 
         * @param _event SFML event
         */
        DrawableContent(sf::Event &_event) : window(nullptr), event(&_event) {};
        /**
         * @brief Drawable Constructor, initialize the window at null
         * 
         * @param _window SFML Render Window
         * @param _event SFML event
         */
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

    /**
     * @brief structure to play Music
     * 
     */
    struct PlayMusic {
        /**
         * @brief Construct a new Play Music object, and plays the music passed as param
         * @param path path to the music to play
         */
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
         * @brief the size of the hitbox
         * 
         */
        Position _size;
        /**
         * @brief Construct a new Hitbox object
         * 
         * @param top_left size of the hitbox to be set
         */
        Hitbox(const Position &size) :
            _size(size)
        {};

        /**
         * @brief check if the two hitboxes overlap
         * 
         * @param p1 posiition x 1
         * @param q1 posiition x 2
         * @param p2 posiition y 1
         * @param q2 posiition y 2
         * @return true 
         * @return false 
         */
        bool isOverlap(float p1, float q1, float p2, float q2) {
            return (p1 < q2) && (q1 > p2);
        };
        /**
         * @brief check if 2 hitboxes are touching
         * 
         * @param pos1 first position to check
         * @param pos2 second position to check
         * @param other other hitbox to check with
         * @return true 
         * @return false 
         */
        bool isTouching(const Position &pos1, const Position &pos2, const Hitbox &other) {
            return (pos1.x < (pos2.x + other._size.x) &&
                    (pos1.x + other._size.x) > pos2.x &&
                    pos1.y < (pos2.y + other._size.y) &&
                    (pos1.y + other._size.y) > pos2.y);
        };
        /**
         * @brief check if hitbox contains position
         * 
         * @param pos position of the hitbox
         * @param x x position to check with
         * @param y y position to check with
         * @return true 
         * @return false 
         */
        bool contains(const Position &pos, int x, int y) {
            Position bottom_right = {_size.x + pos.x, _size.y + pos.y};
            return (x >= pos.x && y >= pos.y && x <= bottom_right.x && y <= bottom_right.y);
        };
        /**
         * @brief equals operator checking current hitbox with other hitbox, compares size
         * 
         * @param other hitbox to compare with
         * @return true 
         * @return false 
         */
        bool operator==(const Hitbox& other) const {
            return _size == other._size;
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

    /**
     * @brief Text stucture to display on screen
     * 
     */
    struct Text {
        /**
         * @brief content of the text
         * 
         */
        std::string _text;
        /**
         * @brief size of the text
         * 
         */
        int _size;
        /**
         * @brief Construct a new Text object
         * 
         * @param text content of the text
         * @param size size of the text
         */
        Text(const std::string &text, int size = 30) : _text(text), _size(size) {};
    };

    /**
     * @brief Parallax structure to know if a Drawable is a parallax
     * 
     */
    struct Parallax {
        /**
         * @brief group id to refer to the correct parallax
         * 
         */
        int _group;
        /**
         * @brief number representing the z-index of the element for the parallax
         * 
         */
        int _index;
        /**
         * @brief Construct a new Parallax object
         * 
         * @param group the group id of the parallax 
         * @param index the z-index of the element for the parallax
         */
        Parallax(int group, int index = 0) : _group(group), _index(index) {};
    };
};

#endif /* !COMPONENTS_HPP_ */