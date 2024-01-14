/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** RoomUtils
*/

#ifndef ROOMUTILS_HPP_
    #define ROOMUTILS_HPP_
    #include <iostream>
    #include <vector>
    #include "SparseArray.hpp"
    #include "Components.hpp"

/**
 * @brief Utility for room related functionnalities
 * 
 */
class RoomUtils {
    public:
        /**
         * @brief Get all of the rooms
         * 
         * @param all_rooms all the rooms under sparse_array format
         * @return std::vector<std::string> of all the rooms
         */
        static std::vector<std::string> GetAll(sparse_array<component::Room> &all_rooms)
        {
            std::vector<std::string> resRooms = {};
            for (size_t i = 0; i < all_rooms.size(); ++i) {
                if (std::find(resRooms.begin(), resRooms.end(), all_rooms[i]->_name) == resRooms.end()) {
                    resRooms.push_back(all_rooms[i]->_name);
                }
            }
            return resRooms;
        }
};

#endif /* !ROOMUTILS_HPP_ */
