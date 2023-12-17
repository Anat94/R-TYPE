/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Events
*/

#include "Events.hpp"

bool rtype::event::EventListener::addEvent(rtype::event::IEvent *event)
{
    _events.push(event);
    return true;
}

bool rtype::event::EventListener::popEvent()
{
    if (!_events.empty()) {
        IEvent* event = _events.front();
        _events.pop();
        
        event->handleEvent(*_reg, *this);
        return true;
    } else
        return false;
}

void rtype::event::EventListener::addRegistry(registry &reg)
{
    _reg = &reg;
}

bool rtype::event::EventListener::hasEvent(rtype::event::IEvent *event)
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

void rtype::event::CollisionEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    try {

        auto &player1_pla = r.get_components<component::Player>()[_ents.first];

        auto &player2_hurt = r.get_components<component::HurtsOnCollision>()[_ents.second];

        player1_pla.value()._health -= player2_hurt.value().damage;
        if (player1_pla.value()._health <= 0) {
            rtype::event::DeathEvent *new_event = new DeathEvent(_ents.first, _ents.second);
            if (listener.hasEvent(new_event))
                delete new_event;
            else {
                std::cout << "Adding Death event" << std::endl;
                listener.addEvent(new_event);
            }
        }
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity does affect player
    }
}

void rtype::event::DeathEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    std::cout << "DeathEvent" << std::endl;

    if (r.entity_exists(_ents.first)) {
        r.remove_component<component::Drawable>(_ents.first);
        r.remove_component<component::Player>(_ents.first);
        r.remove_component<component::Position>(_ents.first);
        r.remove_component<component::Velocity>(_ents.first);
        r.remove_component<component::Controllable>(_ents.first);
        r.kill_entity(_ents.first);
    }

    try {
        // TODO: get the shooter / entity and xp them
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> damager not a player
    }
    std::cout << "tmp" << std::endl;
}

void rtype::event::SpawnEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    // Todo: ping all other to connect new player
}
