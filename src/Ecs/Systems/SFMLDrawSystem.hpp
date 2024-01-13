/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** SFMLDrawSystem
*/

#ifndef SFMLDRAWSYSTEM_HPP_
    #define SFMLDRAWSYSTEM_HPP_
    #include <vector>
    #include <memory>
    #include <unordered_map>
    #include "Systems.hpp"

class SFMLDrawSystem : public ISystems {
    public:
        /**
         * @brief Construct a new SFMLDrawSystem object
         * 
         * @param window window object
         * @param mousePos position of the mouse
         */
        SFMLDrawSystem(sf::RenderWindow *window, sf::Vector2i *mousePos) : _window(window), _mousePos(mousePos), _toDraw() {};

        /**
         * @brief operator ()
         *
         * @param dra draw object
         * @param pos position of the draw object
         * @param hit hit box object
         * @param par parallax object
        */
        void operator()(sparse_array<component::Drawable> &dra, sparse_array<component::Position> &pos, sparse_array<component::Hitbox> &hit, sparse_array<component::Parallax> &par) {
            if (!_window->isOpen())
                return;
            sf::Vector2u windowSize = _window->getSize();
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
                    if (par[i].has_value()) {
                        sf::FloatRect spriteBounds = _toDraw[dra[i]->_path].second->getGlobalBounds();
                        if ((pos[i]->x + spriteBounds.width) <= 0)
                            pos[i]->x += windowSize.x;
                        _toDraw[dra[i]->_path].second->setPosition(pos[i]->x, pos[i]->y);
                        _window->draw((*_toDraw[dra[i]->_path].second));
                        _toDraw[dra[i]->_path].second->setPosition(pos[i]->x + windowSize.x, pos[i]->y);
                        _window->draw((*_toDraw[dra[i]->_path].second));
                    } else {
                        _toDraw[dra[i]->_path].second->setPosition(pos[i]->x, pos[i]->y);
                        _window->draw((*_toDraw[dra[i]->_path].second));
                    }
                }
            }
        };
    private:
        sf::RenderWindow *_window;
        sf::Vector2i *_mousePos;
        std::unordered_map<std::string, std::pair<std::unique_ptr<sf::Texture>, std::unique_ptr<sf::Sprite>>> _toDraw;
};

#endif /* !SFMLDRAWSYSTEM_HPP_ */
