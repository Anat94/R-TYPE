/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SFMLAnimatedDrawSystem
*/

#ifndef SFMLANIMATEDDRAWSYSTEM_HPP_
    #define SFMLANIMATEDDRAWSYSTEM_HPP_
    #include "Systems.hpp"

class SFMLAnimatedDrawSystem : public ISystems {
    public:
        SFMLAnimatedDrawSystem(sf::RenderWindow *window, sf::Vector2i *mousePos) : _window(window), _mousePos(mousePos), _toDraw() {};

        void operator()(sparse_array<component::AnimatedDrawable> &ani, sparse_array<component::Position> &pos, sparse_array<component::Scale> &sca, sparse_array<component::Rotation> &rot)
        {
            float elapsedTick = _clock.getElapsedTime().asMilliseconds();
            for (size_t i = 0; i < ani.size(); ++i) {
                if (ani[i].has_value() && _toDraw.find(i) == _toDraw.end()) {
                    std::unique_ptr<sf::Texture> t = std::make_unique<sf::Texture>();
                    if (!t->loadFromFile(ani[i]->_path)) {
                        std::cout << "Error loading texture" << std::endl;
                        continue;
                    }
                    std::unique_ptr<sf::Sprite> s = std::make_unique<sf::Sprite>();
                    s->setTexture(*t);
                    s->setTextureRect({ani[i]->_firstOffset.first, ani[i]->_firstOffset.second, ani[i]->_spriteSize.first, ani[i]->_spriteSize.second});
                    if (sca[i].has_value())
                        s->setScale(sca[i]->_scale.first, sca[i]->_scale.second);
                    _toDraw.insert(std::make_pair(i, std::move(std::make_pair(ani[i]->_currentIdx, std::move(std::make_pair(std::move(t), std::move(s)))))));
                } else if (ani[i].has_value() && _toDraw.find(i) != _toDraw.end() && elapsedTick >= _threshold) {
                    auto &currentIdx = _toDraw[i].first;
                    if ((currentIdx.first + 1) == ani[i]->_nbSprites.first) {
                        _toDraw[i].second.second->setTextureRect({ani[i]->_firstOffset.first, ani[i]->_firstOffset.second, ani[i]->_spriteSize.first, ani[i]->_spriteSize.second});
                        currentIdx.first = 0;
                    } else {
                        currentIdx.first++;
                        int offsetX = ani[i]->_firstOffset.first + ((ani[i]->_spriteSize.first + ani[i]->_gaps.first) * currentIdx.first);
                        int offsetY = ani[i]->_firstOffset.second;
                        _toDraw[i].second.second->setTextureRect({offsetX, offsetY, ani[i]->_spriteSize.first, ani[i]->_spriteSize.second});
                    }
                    _clock.restart();
                }
            }
            for (size_t i = 0; i < ani.size(); ++i) {
                if (pos[i].has_value() && ani[i].has_value() && _toDraw.find(i) != _toDraw.end()) {
                    _toDraw[i].second.second->setPosition(pos[i]->x, pos[i]->y);
                    _window->draw((*_toDraw[i].second.second));
                }
            }
        };
    private:
        sf::RenderWindow *_window;
        sf::Vector2i *_mousePos;
        std::unordered_map<entity_t, std::pair<std::pair<int, int>, std::pair<std::unique_ptr<sf::Texture>, std::unique_ptr<sf::Sprite>>>> _toDraw;
        sf::Clock _clock;
        float _threshold = 400.0f;
};

#endif /* !SFMLANIMATEDDRAWSYSTEM_HPP_ */
