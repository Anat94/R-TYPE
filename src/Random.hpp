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

class Random {
    public:
        static int generate(int min, int max) {
            std::random_device rd;
            std::mt19937 eng(rd());

            std::uniform_int_distribution<> distribution(min, max);

            return distribution(eng);
        }
};

#endif /* !UTILS_HPP_ */
