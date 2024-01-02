/*
** EPITECH PROJECT, 2023
** R-TYPE
** File description:
** Errors.hpp
*/

#ifndef ERRORS_HPP
#define ERRORS_HPP
#pragma warning(disable: 4668)
#pragma warning(disable: 4626)
#pragma warning(disable: 4625)
#pragma warning(disable: 4820)
#pragma warning(disable: 5031)
#pragma warning(disable: 4365)
#pragma warning(disable: 5027)
#pragma warning(disable: 4514)
#pragma warning(disable: 4464)
#pragma warning(disable: 5026)
#pragma warning(disable: 4457)
#pragma warning(disable: 5262)
#pragma warning(disable: 5204)
#pragma warning(disable: 4355)
#pragma warning(disable: 5220)
#pragma warning(disable: 5039)
#include <iostream>
#include <asio.hpp>

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