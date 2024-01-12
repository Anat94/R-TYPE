/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** ISystems
*/

#ifndef SYSTEMS_HPP_
    #define SYSTEMS_HPP_

    #if not defined(ZIPPERITERATOR_HPP)
        #include "../ZipperIterator.hpp"
    #endif

    #if not defined(REGISTRY_HPP_)
        #include "../Registry.hpp"
    #endif

    #if not defined(EVENT_HPP_)
        #include "../Events.hpp"
    #endif

class ISystems {
    public:
        virtual ~ISystems() = default;
};

#endif /* !SYSTEMS_HPP_ */
