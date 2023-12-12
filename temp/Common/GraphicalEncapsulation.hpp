/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** GraphicalEncapsulation
*/

typedef struct rtype_Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rtype_Color;

#ifndef GRAPHICALENCAPSULATION_HPP_
    #define GRAPHICALENCAPSULATION_HPP_

    #define BASIC_FPS 60
    #define BASIC_SCREEN_WIDTH 1920
    #define BASIC_SCREEN_HEIGHT 1080
    #define SCREEN_NAME "R-type NAPTE"

    #define RTYPE_WHITE (rtype_Color){255, 255, 255}
    #define RTYPE_RED (rtype_Color){255, 0, 0}
    #define RTYPE_YELLOW (rtype_Color){0, 255, 255}
    #define RTYPE_PINK (rtype_Color){255, 255, 255}
    #define RTYPE_BLUE (rtype_Color){0, 0, 255}
    #define RTYPE_GREEN (rtype_Color){0, 255, 0}
    #define RTYPE_PURPLE (rtype_Color){255, 255, 255}
    #define RTYPE_BLACK (rtype_Color){0, 0, 0}
    #define RTYPE_CYAN (rtype_Color){255, 255, 255}
    #define RTYPE_MAGENTA (rtype_Color){255, 255, 255}

    #include <SFML/Graphics.hpp>
    #include <SFML/Audio.hpp>
    #include <raylib.h>
    #include <any>

namespace rtype {

    namespace graphical {

        enum GraphicalLib {
            RAYLIB,
            SFML,
        };

        namespace components {

            struct Position2 {
                float _x, _y;
                Position2(float x, float y) : _x(x), _y(y) {};
            };

            struct Position3 {
                float _x, _y, _z;
                Position3(float x, float y, float z) : _x(x), _y(y), _z(z) {};
            };
        };

        class GraphicalEncapsulation {
            public:
                GraphicalEncapsulation(GraphicalLib lib);
                ~GraphicalEncapsulation();
                void draw(std::any toDraw, components::Position2 pos);
                void window_clear(std::any color);
                void window_display();
                bool handleEvent();

            protected:
            private:
                GraphicalLib _lib;
                void init_SFML();
                void init_RAYLIB();
                sf::RenderWindow _window;
        };
    }
}

#endif /* !GRAPHICALENCAPSULATION_HPP_ */
