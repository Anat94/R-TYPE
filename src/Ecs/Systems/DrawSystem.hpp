/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** DrawSystem
*/

#ifndef DRAWSYSTEM_HPP_
    #define DRAWSYSTEM_HPP_
    #include <vector>
    #include <memory>
    #include <unordered_map>
    #include "Systems.hpp"

class DrawSystem : public ISystems {
    public:
        DrawSystem(sf::RenderWindow *window) : _window(window), _toDraw() {};

        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos) {
            for (size_t i = 0; i < dra.size(); ++i) {
                if (dra[i].has_value() && _toDraw.find(dra[i]->_path) == _toDraw.end()) {
                    std::unique_ptr<sf::Texture> t = std::make_unique<sf::Texture>();
                    if (!t->loadFromFile(dra[i]->_path)) {
                        std::cout << "Error loading texture" << std::endl;
                        continue;
                    }
                    std::unique_ptr<sf::Sprite> s = std::make_unique<sf::Sprite>();
                    s->setTexture(*t);
                    _toDraw.insert(std::make_pair(dra[i]->_path, std::move(std::make_pair(std::move(t), std::move(s)))));
                }
            }
            for (size_t i = 0; i < dra.size(); ++i) {
                if (pos[i].has_value() && dra[i].has_value() && _toDraw.find(dra[i]->_path) != _toDraw.end()) {
                    _toDraw[dra[i]->_path].second->setPosition(pos[i]->x, pos[i]->y);
                    _window->draw((*_toDraw[dra[i]->_path].second));
                }
            }
        };
    private:
        sf::RenderWindow *_window;
        std::unordered_map<std::string, std::pair<std::unique_ptr<sf::Texture>, std::unique_ptr<sf::Sprite>>> _toDraw;
};

#endif /* !DRAWSYSTEM_HPP_ */
