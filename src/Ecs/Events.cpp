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
        IEvent* event = _events.front();
        _events.pop();

        event->handleEvent(*_reg, *this);
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

        player1_pla.value().x += _pos.first;
        player1_pla.value().y += _pos.second;
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
            if (player2_p->_pierce <= 0) {
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
            if (player1_p->_pierce <= 0) {
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
    //! remove player
    try {
        if (r.entity_exists(_ents.first)) {
            r.remove_component<component::Health>(_ents.first);
            r.remove_component<component::Score>(_ents.first);
            r.remove_component<component::Controllable>(_ents.first);
            r.remove_component<component::Scale>(_ents.first);
            r.remove_component<component::Rotation>(_ents.first);
            r.remove_component<component::ResetOnMove>(_ents.first);
            r.remove_component<component::Damage>(_ents.first);
            r.remove_component<component::Heading>(_ents.first);
            r.remove_component<component::Drawable>(_ents.first);
            r.remove_component<component::Position>(_ents.first);
            r.remove_component<component::Velocity>(_ents.first);
            r.kill_entity(_ents.first);
            auto &enemy_score = r.get_components<component::Score>()[_ents.second];
            std::cout << _ents.second << std::endl;
            if (enemy_score.has_value())
                enemy_score->_score += 10;
        }
        return;
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity not a player
    }

    try {
        if (r.entity_exists(_ents.first)) {
            r.remove_component<component::Velocity>(_ents.first);
            r.remove_component<component::Drawable>(_ents.first);
            r.remove_component<component::Position>(_ents.first);
            r.remove_component<component::HurtsOnCollision>(_ents.first);
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
        auto player_p = r.get_components<component::Position>()[_ents.first];
        auto player_h = r.get_components<component::Heading>()[_ents.first];
        auto player_d = r.get_components<component::Damage>()[_ents.first];

        if (player_h.has_value() && player_p.has_value()
            && player_d.has_value()) {
            r.add_component(shot, component::Position((player_p->x + 200), (player_p->y + 30)));
            r.add_component(shot, component::HurtsOnCollision(_ents.first));
            r.add_component(shot, component::Damage(player_d->_damage));
            r.add_component(shot, component::Drawable("./temp/assets/textures/sprites/Hobbit-Idle1.png"));
            if (player_h->_rotation <= 180)
                r.add_component(shot, component::Velocity(5.0f, 0.0f));
            else
                r.add_component(shot, component::Velocity(-5.0f, 0.0f));
        }
    } catch (std::exception &e) {
        //? ignore -> shooter not a player for some reason ???
    }
}
