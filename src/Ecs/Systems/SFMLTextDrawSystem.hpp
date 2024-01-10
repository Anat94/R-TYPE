/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SFMLTextDrawSystem
*/

#ifndef SFMLTEXTDRAWSYSTEM_HPP_
#define SFMLTEXTDRAWSYSTEM_HPP_
    #include "Systems.hpp"

class SFMLTextDrawSystem : public ISystems {
    public:
        SFMLTextDrawSystem(sf::RenderWindow *window) : _window(window), _texts() {
            _font.loadFromFile("src/Client/assets/font.ttf");
        };

        void operator()(sparse_array<component::Text> &tex, sparse_array<component::Position> &pos) {
            if (!_window->isOpen())
                return;
            for (auto &&[idx, t, p] : zipper<sparse_array<component::Text>, sparse_array<component::Position>>(tex, pos)) {
                if (t.has_value() && p.has_value()) {
                    _texts.emplace(idx, sf::Text(t->_text, _font, t->_size));
                    _texts[idx].setPosition(p->x, p->y);
                }
            }

            for (const auto& t : _texts)
                _window->draw(t.second);
        };

    private:
        sf::RenderWindow *_window;
        sf::Font _font;
        std::unordered_map<entity_t, sf::Text> _texts;
};

#endif /* !SFMLTEXTDRAWSYSTEM_HPP_ */
