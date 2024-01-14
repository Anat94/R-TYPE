/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** ISystems
*/

#ifndef SYSTEMS_HPP_
    #define SYSTEMS_HPP_
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define MIN(a, b) ((a) < (b) ? (a) : (b))
    #define IS_IN(a, b, c) ((a) == (b) || (a) == (c))
    #define IS(a, b) ((a) == (b))
    #define IS_NOT(a, b) ((a) != (b))

    #if not defined(ZIPPERITERATOR_HPP)
        #include "../ZipperIterator.hpp"
    #endif

    #if not defined(REGISTRY_HPP_)
        #include "../Registry.hpp"
    #endif

    #if not defined(EVENT_HPP_)
        #include "../Events.hpp"
    #endif

/**
 * @brief Pure Virtual Interface for the Systems
 * 
 */
class ISystems {
    public:
        /**
         * @brief Destroy the ISystems object
         * 
         */
        virtual ~ISystems() = default;
};

#endif /* !SYSTEMS_HPP_ */
