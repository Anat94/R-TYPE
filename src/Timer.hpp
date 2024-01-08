/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** Timer
*/

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

class Timer {
public:
    Timer() : m_start(std::chrono::high_resolution_clock::now()) {}

    void restart() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    float getElapsedTime() const {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> duration = end - m_start;
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

#endif /* !TIMER_HPP_ */
