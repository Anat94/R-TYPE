/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Timer
*/

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

/**
 * @brief Timer class, usefull for timing actions, and checking time elapsed.
*/
class Timer {
public:
    /**
     * @brief Constructor, initializes the clock
    */
    Timer() : m_start(std::chrono::high_resolution_clock::now()) {}

    /**
     * @brief restart the clock
    */
    void restart() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief get elapsed time in milliseconds of the clock
     * 
     * @returns milliseconds elapsed since the clock started.
    */
    float getElapsedTime() const {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = end - m_start;
        return duration.count();
    }

private:
    /**
     * @brief parameter container the time when the clock started.
    */
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif /* !TIMER_HPP_ */
