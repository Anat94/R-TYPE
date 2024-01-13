/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Events
*/

#include "Events.hpp"

/**
 * @brief  add an event to the queue
 *
 * @param event event to add
 * @return true if the event was added
 */
bool EventListener::addEvent(IEvent *event)
{
    _events.push(event);
    return true;
}

/**
 * @brief  pop the first event of the queue and handle it
 *
 * @return true if an event was poped
 * @return false if the queue is empty
 */
bool EventListener::popEvent()
{
    if (!_events.empty()) {
        IEvent* event = _events.front();
        _events.pop();
        event->handleEvent(*_reg, *this);
        return true;
    } else
        return false;
}

/**
 * @brief add a registry to the event listener
 * 
 * @param reg reference to the registry
 */
void EventListener::addRegistry(registry &reg)
{
    _reg = &reg;
}

/**
 * @brief has the event in the queue
 *
 * @param event event to check
 * @return true if the event is in the queue
 * @return false if the event is not in the queue
 */
bool EventListener::hasEvent(IEvent *event)
{
    const std::type_info& eventType = typeid(*(event));
    std::queue<IEvent*> tempQueue = _events;

    while (!tempQueue.empty()) {
        if (*tempQueue.front() == *event &&
            typeid(*tempQueue.front()) == eventType)
            return true;
        tempQueue.pop();
    }
    return false;
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void UpdatePositionEvent::handleEvent(registry &r, EventListener &listener)
{
    try {

        auto &player1_pla = r.get_components<component::Position>()[_to_move];

        if (player1_pla.has_value()) {
            player1_pla->x += _pos.first;
            player1_pla->y += _pos.second;
        }
    } catch (const std::exception &e) {
        e.what();
    }
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void PositionStayInWindowBounds::handleEvent(registry &r, EventListener &listener)
{
    try {
        auto &pos = r.get_components<component::Position>()[_ents.first];

        if (pos->x < _windowBounds[0])
            pos->x = _windowBounds[0];
        if (pos->x > _windowBounds[1])
            pos->x = _windowBounds[1];
        if (pos->y < _windowBounds[2])
            pos->y = _windowBounds[2];
        if (pos->y > _windowBounds[3])
            pos->y = _windowBounds[3];
    } catch (const std::exception &e) {
        e.what();
    }
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void CollisionEvent::handleEvent(registry &r, EventListener &listener)
{
    try {
        auto &player1_h = r.get_components<component::Health>()[_ents.first];

        auto &player2_hurt = r.get_components<component::HurtsOnCollision>()[_ents.second];
        auto &player2_d = r.get_components<component::Damage>()[_ents.second];
        auto &player2_p = r.get_components<component::Pierce>()[_ents.second];

        if (_ents.first == player2_hurt->_sender)
            return;

        if (player1_h.has_value() && player2_hurt.has_value()
            && player2_d.has_value()) {
            player1_h->_health -= player2_d->_damage;
            if (player1_h->_health <= 0) {
                DeathEvent *new_event = new DeathEvent(_ents.first, player2_hurt->_sender);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
            player2_p->_pierce -= 1;
            if (player2_p->_pierce == 0) {
                DeathEvent *new_event = new DeathEvent(_ents.second, -1);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
        }
        return;
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity does affect player
    }

    try {
        auto &player1_hurt = r.get_components<component::HurtsOnCollision>()[_ents.first];
        auto &player1_p = r.get_components<component::Pierce>()[_ents.first];
        auto &player1_d = r.get_components<component::Damage>()[_ents.first];

        auto &player2_h = r.get_components<component::Health>()[_ents.second];

        if (player1_hurt.has_value() && player2_h.has_value()
            && player1_p.has_value() && player1_d.has_value()) {
            player2_h->_health -= player1_d->_damage;
            if (player2_h->_health <= 0) {
                DeathEvent *new_event = new DeathEvent(_ents.second, player1_hurt->_sender);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
            player1_p->_pierce -= 1;
            if (player1_p->_pierce == 0) {
                DeathEvent *new_event = new DeathEvent(_ents.first, -1);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
        }
        return;
    } catch (const std::exception &e) {
        e.what();
        //? ignore
    }
}

/**
 * @brief event to handle the death of an entity
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void DeathEvent::handleEvent(registry &r, EventListener &listener)
{
    //! remove enemy
    try {
        r.get_components<component::Controllable>()[_ents.first]; // see if the entity is the player
        if (r.entity_exists(_ents.first)) {
            r.remove_component<component::Position>(_ents.first);
            r.remove_component<component::Velocity>(_ents.first);
            r.remove_component<component::Scale>(_ents.first);
            r.remove_component<component::Health>(_ents.first);
            r.remove_component<component::Damage>(_ents.first);
            r.remove_component<component::Hitbox>(_ents.first);
            r.remove_component<component::AnimatedDrawable>(_ents.first);
            r.remove_component<component::ServerEntity>(_ents.first);
            r.kill_entity(_ents.first);
            auto &killer_score = r.get_components<component::Score>()[_ents.second];
            if (killer_score.has_value())
                killer_score->_score += 10;
        }
        return;
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity not a player
    }
    try {
        if (r.entity_exists(_ents.first)) {
            r.remove_component<component::Velocity>(_ents.first);
            r.remove_component<component::AnimatedDrawable>(_ents.first);
            r.remove_component<component::Position>(_ents.first);
            r.remove_component<component::ServerEntity>(_ents.first);
            r.remove_component<component::Scale>(_ents.first);
            r.remove_component<component::Health>(_ents.first);
            r.remove_component<component::Room>(_ents.first);
            r.remove_component<component::Hitbox>(_ents.first);
            r.remove_component<component::AnimatedDrawable>(_ents.first);
            r.remove_component<component::HurtsOnCollision>(_ents.first);
            r.remove_component<component::Damage>(_ents.first);
            r.remove_component<component::Pierce>(_ents.first);

            r.kill_entity(_ents.first);
        }
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity not a projectile
    }
}

/**
 * @brief event to handle the death of an entity
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void SpawnEvent::handleEvent(registry &r, EventListener &listener)
{
    // Todo: ping all other to connect new player
}

/**
 * @brief event to handle the spawn of an entity
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void SpawnEnemy::handleEvent(registry &r, EventListener &listener)
{
    entity_t enemy = r.spawn_entity();

    r.add_component<component::Position>(enemy, component::Position(_pos.x, _pos.y));
    r.add_component<component::Velocity>(enemy, component::Velocity(_vel._dx, _vel._dy));
    r.add_component<component::Scale>(enemy, component::Scale(_scale));
    r.add_component<component::Health>(enemy, component::Health(_health));
    if (_roomName.size() != 0)
        r.add_component<component::Room>(enemy, component::Room(_roomName));
    r.add_component<component::Hitbox>(enemy, component::Hitbox(component::Position(_animatedDrawable._spriteSize.first * _scale, _animatedDrawable._spriteSize.second * _scale)));
    r.add_component<component::AnimatedDrawable>(enemy, component::AnimatedDrawable(_animatedDrawable._path, _animatedDrawable._nbSprites, _animatedDrawable._spriteSize, _animatedDrawable._gaps, _animatedDrawable._firstOffset, _animatedDrawable._currentIdx));

    auto &tmp1 = r.get_components<component::AnimatedDrawable>()[enemy];
    for (auto & anim: _anims) {
        tmp1->addAnimation(anim.first, anim.second.first, anim.second.second);
    }
    tmp1->_state = "idle";
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void ShootEvent::handleEvent(registry &r, EventListener &listener)
{
    entity_t shot = r.spawn_entity();

    try {
        auto player_hit = r.get_components<component::Hitbox>()[_ents.first];
        auto player_p = r.get_components<component::Position>()[_ents.first];
        auto player_h = r.get_components<component::Heading>()[_ents.first];
        auto player_d = r.get_components<component::Damage>()[_ents.first];
        auto player_room = r.get_components<component::Room>()[_ents.first];

        if (player_hit.has_value() && player_d.has_value() && player_h.has_value() && player_p.has_value()) {
            component::Position top_left = component::Position(((player_p->x + player_hit->_size.x) + 1), (player_p->y - ((player_hit->_size.y) / 2)));
            r.add_component(shot, component::Position(top_left.x, top_left.y));
            r.add_component(shot, component::HurtsOnCollision(_ents.first));
            r.add_component(shot, component::Damage(player_d->_damage));
            r.add_component(shot, component::Scale(2.0f));
            if (player_room.has_value())
                r.add_component<component::Room>(shot, component::Room(player_room->_name));
            r.add_component(shot, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet1.gif", {4, 0}, {32, 32}, {1, 0}, {136, 18}));
            auto &tmp = r.get_components<component::AnimatedDrawable>()[shot];
            tmp->addAnimation("idle", {0, 3}, true);
            tmp->_state = "idle";
            r.add_component(shot, component::Hitbox(component::Position(32 * 4.0f, 32 * 4.0f)));
            r.add_component(shot, component::Pierce());
            if (player_h->_rotation <= 180)
                r.add_component(shot, component::Velocity(32.0f, 0.0f));
            else
                r.add_component(shot, component::Velocity(-32.0f, 0.0f));
        }
    } catch (std::exception &e) {
        //? ignore -> shooter not a player for some reason ???
    }
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void ClickBtnEvent::handleEvent(registry &r, EventListener &listener)
{
    // lance la fonction qui est passer en seconde dans _ents
}

/**
 * @brief handle the event
 *
 * @param r registry to use
 * @param listener event listener to use
 */
void HoverBtnEvent::handleEvent(registry &r, EventListener &listener)
{
    // chnage la couleur du bouton
}