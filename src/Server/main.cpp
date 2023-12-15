/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"
#include "../Errors.hpp"

using boost::asio::ip::udp;

int error_handling(int nb_args)
{
    if (nb_args != 2)
        throw ArgumentError("./server <server_port>");
    return 0;
}

int main(int argc, char** argv)
{
    try {
        error_handling(argc);
        boost::asio::io_service service;
        Server server(service, std::atoi(argv[1]));
        service.run();
        return 0;
    } catch (ArgumentError e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    }
}