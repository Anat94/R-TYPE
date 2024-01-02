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
            std::cout << dra.size() << " sprites to draw" << std::endl;

            for (size_t i = 0; i < dra.size(); ++i) {
                if (dra[i].has_value()) {
                    std::cout << dra[i]->_path << std::endl;
                    std::shared_ptr<sf::Texture> t = std::make_shared<sf::Texture>();
                    if (!t->loadFromFile(dra[i]->_path)) {
                        std::cout << "Error loading texture" << std::endl;
                        continue;
                    }
                    std::shared_ptr<sf::Sprite> s = std::make_shared<sf::Sprite>();
                    s->setTexture(*t);
                    std::cout << "texture" << std::endl;
                    // _toDraw.insert(std::move(std::make_pair(dra[i]->_path, std::move(std::make_pair(std::move(t), std::move(s))))));
                    std::cout << dra[i]->_path << std::endl;
                    _toDraw.emplace(
                        std::make_pair(
                            dra[i]->_path, 
                            std::move(std::make_pair(t, s))
                        )
                    );
                    // auto &test = _toDraw[dra[i]->_path];
                    std::cout << "prout" << std::endl;
                    // test = std::make_pair(t, s);
                    std::cout << "texture afer" << std::endl;
                }
            }
            std::cout << _toDraw.size() << "  jkn re r,;rtklnsprites to draw" << std::endl;
            // for (size_t i = 0; i < dra.size(); ++i) {
            //     std::cout << "Drawing " << i << " times" << std::endl;
            //     if (_toDraw[i] && _toDraw[i].has_value())
            //         continue;
            //     if (_toDraw[i] && dra[i].has_value()) {
            //         _toDraw[i]->_path = dra[i]->_path;
            //         _toDraw[i]->_texture.loadFromFile(dra[i]->_path);
            //         _toDraw[i]->_sprite.setTexture(_toDraw[i]->_texture);
            //     }
            // }
            // std::cout << "Draw system second" << std::endl;
            // for (auto &&[_, d, p] : zipper<sparse_array<component::Drawable>, sparse_array<component::Position>>(_toDraw, pos)) {
            //     if (d.has_value() && p.has_value()) {
            //         std::cout << "Found sprite to draw: " + d->_path << std::endl;
            //         d->_sprite.setPosition(p->x, p->y);
            //         _window->draw(d->_sprite);
            //     }
            // }
        };
    private:
        sf::RenderWindow *_window;
        std::unordered_map<std::string, std::pair<std::shared_ptr<sf::Texture>, std::shared_ptr<sf::Sprite>>> _toDraw;
};

#endif /* !DRAWSYSTEM_HPP_ */
