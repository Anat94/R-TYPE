/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Utils
*/

#ifndef UTILS_HPP_
    #define UTILS_HPP_
    #include <iostream>
    #include <random>

/**
 * @brief Random class that contains operations on random numbers
*/
class Random {
    public:
        /**
         * @brief Generate a random whole number between min & max
         * 
         * @param min minimum of the range in which to generate a random number
         * @param max maximum of the range in which to generate a random number
         * 
         * @returns random integer between min & max
        */
        static int generate(int min, int max) {
            std::random_device rd;
            std::mt19937 eng(rd());

            std::uniform_int_distribution<> distribution(min, max);

            return distribution(eng);
        }
};

#endif /* !UTILS_HPP_ */
