/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Errors.hpp
*/

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <iostream>
#include <boost/asio.hpp>

class Error : public std::exception{
    public:
        Error(std::string message) : _message(message) {}
        ~Error() {};

        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        std::string _message;
};

class ArgumentError : public std::exception{
    public:
        ArgumentError(std::string message) : _message(message) {}
        ~ArgumentError() {};

        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        std::string _message;
};

class SFMLError : public std::exception{
    public:
        SFMLError(std::string message) : _message(message) {}
        ~SFMLError() {};

        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        std::string _message;
};

class DatabaseError : public std::exception{
    public:
        DatabaseError(std::string message) : _message(message) {}
        ~DatabaseError() {};

        const char *what() const noexcept { return _message.c_str(); };

    protected:
    private:
        std::string _message;
};

#endif //ERRORS_HPP