/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** logger.hpp
*/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>
#include <fstream>

enum type {
    CLIENT,
    SERVER,
};

class Logger {
    public:
        Logger(type _type) {
            std::ofstream file;
            if (_type == CLIENT)
                file.open("log/client.log", std::ios::out);
            else
                file.open("log/server.log", std::ios::out);
            file.close();
        }
        ~Logger() {}

        std::string getTimeStamp() {
            time_t now = time(0);
            tm *ltm = localtime(&now);
            std::string timestamp = "[" + std::to_string(ltm->tm_mday) + "/" + std::to_string(1 + ltm->tm_mon) + "/" + std::to_string(1900 + ltm->tm_year) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec) + "]";
            return timestamp;
        }
        void log(type _type, std::string message) {
            std::ofstream file;
            if (_type == CLIENT)
                file.open("log/client.log", std::ios::app);
            else
                file.open("log/server.log", std::ios::app);
            file << getTimeStamp() << " " << message << std::endl;
            file.close();
        }
    private:
};

#endif //LOGGER_HPP_