/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Events
*/

#include "Events.hpp"

bool EventListener::addEvent(IEvent *event)
{
    _events.push(event);
    return true;
}

bool EventListener::popEvent()
{
    if (!_events.empty()) {
        // _reg->can_run_updates = false;
        IEvent* event = _events.front();
        _events.pop();
        event->handleEvent(*_reg, *this);
        // _reg->can_run_updates = true;
        return true;
    } else
        return false;
}

void EventListener::addRegistry(registry &reg)
{
    _reg = &reg;
}

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

void UpdatePositionEvent::handleEvent(registry &r, EventListener &listener)
{
    try {

        auto &player1_pla = r.get_components<component::Position>()[_to_move];

        if (player1_pla.has_value()) {
            player1_pla->x += _pos.first;
            player1_pla->y += _pos.second;
        }
        std::cout << "updated position: x: " << player1_pla.value().x << ", y: " << player1_pla.value().y << std::endl;
    } catch (const std::exception &e) {
        e.what();
    }
}

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
            r.kill_entity(_ents.first);
        }
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity not a projectile
    }
}

void SpawnEvent::handleEvent(registry &r, EventListener &listener)
{
    // Todo: ping all other to connect new player
}

void ShootEvent::handleEvent(registry &r, EventListener &listener)
{
    entity_t shot = r.spawn_entity();

    try {
        auto player_hit = r.get_components<component::Hitbox>()[_ents.first];
        auto player_p = r.get_components<component::Position>()[_ents.first];
        auto player_h = r.get_components<component::Heading>()[_ents.first];
        auto player_d = r.get_components<component::Damage>()[_ents.first];

        if (player_hit.has_value() && player_d.has_value() && player_h.has_value() && player_p.has_value()) {
            component::Position top_left = component::Position(((player_p->x + player_hit->_size.x) + 1), (player_p->y - ((player_hit->_size.y) / 2)));
            r.add_component(shot, component::Position(top_left.x, top_left.y));
            r.add_component(shot, component::HurtsOnCollision(_ents.first));
            r.add_component(shot, component::Damage(player_d->_damage));
            r.add_component(shot, component::Scale(4.0f));
            r.add_component(shot, component::AnimatedDrawable("temp/assets/textures/sprites/r-typesheet1.gif", {4, 0}, {32, 32}, {1, 0}, {136, 18}));
            auto &tmp = r.get_components<component::AnimatedDrawable>()[shot];
            tmp->addAnimation("idle", {0, 3}, true);
            tmp->_state = "idle";
            r.add_component(shot, component::Hitbox(component::Position(32 * 4.0f, 32 * 4.0f)));
            r.add_component(shot, component::Pierce());
            if (player_h->_rotation <= 180)
                r.add_component(shot, component::Velocity(12.0f, 0.0f));
            else
                r.add_component(shot, component::Velocity(-12.0f, 0.0f));
        }
    } catch (std::exception &e) {
        //? ignore -> shooter not a player for some reason ???
    }
}

void ClickBtnEvent::handleEvent(registry &r, EventListener &listener)
{
    // lance la fonction qui est passer en seconde dans _ents
}

void HoverBtnEvent::handleEvent(registry &r, EventListener &listener)
{
    // chnage la couleur du bouton
}