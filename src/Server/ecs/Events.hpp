/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** Event
*/

#ifndef EVENT_HPP_
    #define EVENT_HPP_
    #include <queue>
    #include "Registry.hpp"

/**
 * @brief Namespace for the project
 * 
 */
namespace rtype {

    /**
     * @brief Namespace for the project's events
     * 
     */
    namespace event {

        /**
         * @brief Pre-definition of the EventListener class
         * 
         */
        class EventListener;

        /**
         * @brief Main event interface, used for creating events instances
         * 
         */
        class IEvent {
            public:
                /**
                 * @brief Destroy the IEvent object
                 * 
                 */
                virtual ~IEvent() = default;
                /**
                 * @brief Returns the two main objects id responsible for the event
                 * 
                 * @return std::pair<entity_t, entity_t> 
                 */
                virtual std::pair<entity_t, entity_t> get() const = 0;
                virtual bool operator==(const IEvent &other) const = 0;
                virtual void handleEvent(registry &r, EventListener &listener) = 0;
        };

        /**
         * @brief Abstract class for events
         * 
         * Implementing operator==
         */
        class AEvent : public IEvent {
            public:
                virtual ~AEvent() = default;
                std::pair<entity_t, entity_t> get() const { return _ents; }
                /**
                 * @brief operator== allows comparison between two events
                 * 
                 * @param other The even to compare with the actual one
                 * @return true if the values of the events are equals,
                 * @return false otherwise
                 */
                bool operator==(const IEvent &other) const override
                {
                    if (dynamic_cast<const AEvent*>(&other) != nullptr) {
                        const AEvent &otherAEvent = dynamic_cast<const AEvent&>(other);
                        return _ents == otherAEvent._ents;
                    }
                    return false;
                };
                /**
                 * @brief Main function of the Event, used to aply changes on the registery passed as paramter
                 * 
                 * @param r registery_t object, containig the game instance and objects
                 * @param listener the event listener, used to create new events from this one when needed
                 */
                virtual void handleEvent(registry &r, EventListener &listener) = 0;

            protected:
                /**
                 * @brief The two main objects id responsible for the event
                 * 
                 */
                std::pair<entity_t, entity_t> _ents;

        };

        /**
         * @brief Real class declaration
         * 
         */
        class EventListener {
            public:
                /**
                 * @brief Construct a new Event Listener object
                 * 
                 */
                EventListener() {};
                /**
                 * @brief Destroy the Event Listener object
                 * 
                 */
                ~EventListener() = default;
                /**
                 * @brief Adds an event passed as parameter to the queue of events
                 * 
                 * @param event the event to add
                 * @return true if the event was successfully added,
                 * @return false otherwise
                 */
                bool addEvent(IEvent *event);
                /**
                 * @brief Removes the first event in the queue of events
                 * 
                 * @return true if the event whas successfully removed and executed,
                 * @return false otherwise
                 */
                bool popEvent();
                /**
                 * @brief Stores the regitry_t object inside of the listener
                 * 
                 * @param reg the registry_t instance pointer
                 */
                void addRegistry(registry &reg);
                /**
                 * @brief Checks if the listener currently has the event passed as parameter
                 * 
                 * @param event 
                 * @return true 
                 * @return false 
                 */
                bool hasEvent(IEvent *event);
                /**
                 * @brief The (FIFO) queue of events
                 * 
                 */
                std::queue<IEvent*> _events;
            private:
                /**
                 * @brief Pointer to the registery object used to store the objects of the game engine
                 * 
                 */
                registry *_reg;
        };

        /**
         * @brief Collision event class
         * 
         */
        class UpdatePositionEvent : public AEvent {
            public:
                /**
                 * @brief Construct a new Collision Event object
                 * 
                 * @param gotTouched entity id that got touched in the event
                 * @param touched entity id that touched during the event
                 */
                UpdatePositionEvent(entity_t to_move, std::pair<int, int> pos) { _to_move = {to_move}; _pos = pos; };
                /**
                 * @brief Handles the event based on the registry objects
                 * 
                 * @param r the registry_t object used to store the game engine resources
                 * @param listener the event listener used to create new events if needed
                 */
                void handleEvent(registry &r, EventListener &listener);
            private:
                entity_t _to_move;
                std::pair<int, int> _pos;
        };

        /**
         * @brief Collision event class
         * 
         */
        class CollisionEvent : public AEvent {
            public:
                /**
                 * @brief Construct a new Collision Event object
                 * 
                 * @param gotTouched entity id that got touched in the event
                 * @param touched entity id that touched during the event
                 */
                CollisionEvent(entity_t gotTouched, entity_t touched) { _ents = {gotTouched, touched}; };
                /**
                 * @brief Handles the event based on the registry objects
                 * 
                 * @param r the registry_t object used to store the game engine resources
                 * @param listener the event listener used to create new events if needed
                 */
                void handleEvent(registry &r, EventListener &listener);
        };

        /**
         * @brief Death event class
         * 
         */
        class DeathEvent : public AEvent {
            public:
                /**
                 * @brief Construct a new Death Event object
                 * 
                 * @param gotTouched the entity id that got touched in the event
                 * @param touched the entity id that touched during the event
                 */
                DeathEvent(entity_t gotTouched, entity_t touched) { _ents = {gotTouched, touched}; };
                /**
                 * @brief Handles the event based on the registry objects
                 * 
                 * @param r the registry_t object used to store the game engine resources
                 * @param listener the event listener used to create new events if needed
                 */
                void handleEvent(registry &r, EventListener &listener);
        };

        /**
         * @brief Spawn event class
         * 
         */
        class SpawnEvent : public AEvent {
            public:
                /**
                 * @brief Construct a new Spawn Event object
                 * 
                 * @param spawned the entity id that got spawned in the event
                 * @param pos the position id of the newly spawned entity
                 */
                SpawnEvent(entity_t spawned, entity_t pos) { _ents = {spawned, pos}; };
                /**
                 * @brief Handles the event based on the registry objects
                 * 
                 * @param r the registry_t object used to store the game engine resources
                 * @param listener the event listener used to create new events if needed
                 */
                void handleEvent(registry &r, EventListener &listener);
        };

        /**
         * @brief Shoot event class
         * 
         */
        class ShootEvent : public AEvent {
            public:
                /**
                 * @brief Construct a new Shoot Event object
                 * 
                 * @param shooter the entity id that shot in the event
                 * @param projectile the entity id that got spawned during the event
                 */
                ShootEvent(entity_t shooter, entity_t projectile) { _ents = {shooter, projectile}; };

                /**
                 * @brief Handles the event based on the registry objects
                 * 
                 * @param r the registry_t object used to store the game engine resources
                 * @param listener the event listener used to create new events if needed
                 */
                void handleEvent(registry &r, EventListener &listener);
        };
    };
}

#endif /* !EVENT_HPP_ */
