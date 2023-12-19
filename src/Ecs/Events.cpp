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
        std::cout << "player1_pla : " << _ents.first << std::endl;
        std::cout << "player1_pla : " << _ents.second << std::endl;
        std::cout << "player1_pla : " << player2_hurt.value()._sender << std::endl;
        if (player1_pla.value()._health <= 0) {
            rtype::event::DeathEvent *new_event = new DeathEvent(_ents.first, player2_hurt->_sender);
            if (listener.hasEvent(new_event))
                delete new_event;
            else
                listener.addEvent(new_event);
        }
        player2_hurt->_pierce -= 1;
        if (player2_hurt->_pierce <= 0) {
            rtype::event::DeathEvent *new_event = new DeathEvent(_ents.second, -1);
            if (listener.hasEvent(new_event))
                delete new_event;
            else
                listener.addEvent(new_event);
        }
        return;
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> entity does affect player
    }

    try {
        auto &player2_pla = r.get_components<component::Player>()[_ents.second];

        auto &player1_hurt = r.get_components<component::HurtsOnCollision>()[_ents.first];

        if (player1_hurt.has_value() && player2_pla.has_value()) {
            player2_pla.value()._health -= player1_hurt.value().damage;
            if (player2_pla.value()._health <= 0) {
                rtype::event::DeathEvent *new_event = new DeathEvent(player1_hurt.value()._sender, _ents.second);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
            player1_hurt->_pierce -= 1;
            if (player1_hurt->_pierce <= 0) {
                rtype::event::DeathEvent *new_event = new DeathEvent(_ents.second, -1);
                if (listener.hasEvent(new_event))
                    delete new_event;
                else
                    listener.addEvent(new_event);
            }
        }
        return;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        //? ignore
    }
}

void rtype::event::DeathEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    //! remove player
    try {
        printf("ds try\n");
        std::cout << _ents.first <<"," << _ents.second << std::endl;
        if (r.entity_exists(_ents.first)) {
            r.remove_component<component::Player>(_ents.first);
            r.remove_component<component::Drawable>(_ents.first);
            r.remove_component<component::Position>(_ents.first);
            r.remove_component<component::Velocity>(_ents.first);
            r.kill_entity(_ents.first);
            auto &comp = r.get_components<component::Player>()[_ents.second];
            std::cout << _ents.second << std::endl;
            if (!comp.has_value())
                return;
            std::cout << "xp : " << comp.value()._xp <<  std::endl;
            comp.value()._xp += 10;
            std::cout << "xp : " << comp.value()._xp <<  std::endl;
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

    try {
        // TODO: get the shooter / entity and xp them
    } catch (const std::exception &e) {
        e.what();
        //? ignore -> damager not a player
    }
}

void rtype::event::SpawnEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    // Todo: ping all other to connect new player
}

void rtype::event::ShootEvent::handleEvent(registry &r, rtype::event::EventListener &listener)
{
    entity_t shot = r.spawn_entity();
    std::cout << "shoot=" << shot << std::endl;

    try {
        auto player = r.get_components<component::Player>()[_ents.first];
        auto player_pos = r.get_components<component::Position>()[_ents.first];
        auto player_heading = r.get_components<component::Heading>()[_ents.first];

        if (player_heading.has_value() && player_pos.has_value() && player.has_value()) {
            r.add_component(shot, component::Position((player_pos->x + 100), (player_pos->y + 50)));
            r.add_component(shot, component::HurtsOnCollision(200, _ents.first));
            r.add_component(shot, component::Drawable("./temp/assets/textures/sprites/Hobbit-Idle1.png"));
            if (player_heading->_rotation <= 180)
                r.add_component(shot, component::Velocity(5.0f, 0.0f));
            else
                r.add_component(shot, component::Velocity(-5.0f, 0.0f));
        }
    } catch (std::exception &e) {
        //? ignore -> shooter not a player for some reason ???
    }
}
