/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** FilesUtils
*/

#ifndef FILESUTILS_HPP_
    #define FILESUTILS_HPP_
    #include <iostream>
    #include <fstream>

class FilesUtils {
    public:
        static std::string readJsonFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Error opening file: " + filename);
            }

            return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        }
};

#endif /* !FILESUTILS_HPP_ */
