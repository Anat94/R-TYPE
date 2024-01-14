/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Errors.hpp
*/

#ifndef ERRORS_HPP
#define ERRORS_HPP
//#pragma warning(disable: 4668)
//#pragma warning(disable: 4626)
//#pragma warning(disable: 4625)
//#pragma warning(disable: 4820)
//#pragma warning(disable: 5031)
//#pragma warning(disable: 4365)
//#pragma warning(disable: 5027)
//#pragma warning(disable: 4514)
//#pragma warning(disable: 4464)
//#pragma warning(disable: 5026)
//#pragma warning(disable: 4457)
//#pragma warning(disable: 5262)
//#pragma warning(disable: 5204)
//#pragma warning(disable: 4355)
//#pragma warning(disable: 5220)
//#pragma warning(disable: 5039)
#include <iostream>
#include <asio.hpp>

/**
 * @brief Custom Error class
*/
class Error : public std::exception{
    public:
        /**
         * @brief Constructor to call in a "throw"
         * 
         * @param message error message
        */
        Error(std::string message) : _message(message) {}
        /**
         * @brief Destroys the structure
        */
        ~Error() {};

        /**
         * @brief used to get the error message.
         * 
         * @returns string containing the error message.
        */
        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        /**
         * @brief error message
        */
        std::string _message;
};

/**
 * @brief Custom Argument Error class
*/
class ArgumentError : public std::exception{
    public:
        /**
         * @brief Constructor to call in a "throw"
         * 
         * @param message error message
        */
        ArgumentError(std::string message) : _message(message) {}
        /**
         * @brief Destroys the structure
        */
        ~ArgumentError() {};

        /**
         * @brief used to get the error message.
         * 
         * @returns string containing the error message.
        */
        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        /**
         * @brief error message
        */
        std::string _message;
};

/**
 * @brief Custom SFML Error class
*/
class SFMLError : public std::exception{
    public:
        /**
         * @brief Constructor to call in a "throw"
         * 
         * @param message error message
        */
        SFMLError(std::string message) : _message(message) {}
        /**
         * @brief Destroys the structure
        */
        ~SFMLError() {};

        /**
         * @brief used to get the error message.
         * 
         * @returns string containing the error message.
        */
        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        /**
         * @brief error message
        */
        std::string _message;
};

/**
 * @brief Custom Database Error class
*/
class DatabaseError : public std::exception{
    public:
        /**
         * @brief Constructor to call in a "throw"
         * 
         * @param message error message
        */
        DatabaseError(std::string message) : _message(message) {}
        /**
         * @brief Destroys the structure
        */
        ~DatabaseError() {};

        /**
         * @brief used to get the error message.
         * 
         * @returns string containing the error message.
        */
        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        /**
         * @brief error message
        */
        std::string _message;
};

#endif //ERRORS_HPP