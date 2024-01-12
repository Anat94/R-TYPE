/*
** EPITECH PROJECT, 2024
** rtype
** File description:
** KeyEventMapping
*/

#ifndef KEYEVENTMAPPING_HPP_
    #define KEYEVENTMAPPING_HPP_
    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <unordered_map>

/**
 * @brief mapping of keyboard keys to an integer representing their ID
*/
static std::unordered_map<std::string, int> KeyIds = {
    {"A", 0},
    {"B", 1},
    {"C", 2},
    {"D", 3},
    {"E", 4},
    {"F", 5},
    {"G", 6},
    {"H", 7},
    {"I", 8},
    {"J", 9},
    {"K", 10},
    {"L", 11},
    {"M", 12},
    {"N", 13},
    {"O", 14},
    {"P", 15},
    {"Q", 16},
    {"R", 17},
    {"S", 18},
    {"T", 19},
    {"U", 20},
    {"V", 21},
    {"W", 22},
    {"X", 23},
    {"Y", 24},
    {"Z", 25},
    {"Num0", 26},
    {"Num1", 27},
    {"Num2", 28},
    {"Num3", 29},
    {"Num4", 30},
    {"Num5", 31},
    {"Num6", 32},
    {"Num7", 33},
    {"Num8", 34},
    {"Num9", 35},
    {"Space", 36},
    {"Backspace", 37},
    {"Comma", 38},
    {"SemiColon", 39},
    {"Enter", 40},
    {"Left", 41},
    {"Right", 42},
    {"Up", 43},
    {"Down", 44},
    {"Lshift", 45},
    {"Tab", 46},
};

/**
 * @brief map of SFML events mapped to keyboard keys.
*/
static std::unordered_map<sf::Keyboard::Key, std::string> SFMLKeys = {
    {sf::Keyboard::A, "A"},
    {sf::Keyboard::B, "B"},
    {sf::Keyboard::C, "C"},
    {sf::Keyboard::D, "D"},
    {sf::Keyboard::E, "E"},
    {sf::Keyboard::F, "F"},
    {sf::Keyboard::G, "G"},
    {sf::Keyboard::H, "H"},
    {sf::Keyboard::I, "I"},
    {sf::Keyboard::J, "J"},
    {sf::Keyboard::K, "K"},
    {sf::Keyboard::L, "L"},
    {sf::Keyboard::M, "M"},
    {sf::Keyboard::N, "N"},
    {sf::Keyboard::O, "O"},
    {sf::Keyboard::P, "P"},
    {sf::Keyboard::Q, "Q"},
    {sf::Keyboard::R, "R"},
    {sf::Keyboard::S, "S"},
    {sf::Keyboard::T, "T"},
    {sf::Keyboard::U, "U"},
    {sf::Keyboard::V, "V"},
    {sf::Keyboard::W, "W"},
    {sf::Keyboard::X, "X"},
    {sf::Keyboard::Y, "Y"},
    {sf::Keyboard::Z, "Z"},
    {sf::Keyboard::Num0, "Num0"},
    {sf::Keyboard::Num1, "Num1"},
    {sf::Keyboard::Num2, "Num2"},
    {sf::Keyboard::Num3, "Num3"},
    {sf::Keyboard::Num4, "Num4"},
    {sf::Keyboard::Num5, "Num5"},
    {sf::Keyboard::Num6, "Num6"},
    {sf::Keyboard::Num7, "Num7"},
    {sf::Keyboard::Num8, "Num8"},
    {sf::Keyboard::Num9, "Num9"},
    {sf::Keyboard::Space, "Space"},
    {sf::Keyboard::BackSpace, "Backspace"},
    {sf::Keyboard::Comma, "Comma"},
    {sf::Keyboard::SemiColon, "SemiColon"},
    {sf::Keyboard::Enter, "Enter"},
    {sf::Keyboard::Left, "Left"},
    {sf::Keyboard::Right, "Right"},
    {sf::Keyboard::Up, "Up"},
    {sf::Keyboard::Down, "Down"},
    {sf::Keyboard::LShift, "Lshift"},
    {sf::Keyboard::Tab, "Tab"},
};

#endif /* !KEYEVENTMAPPING_HPP_ */
