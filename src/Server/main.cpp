/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>
#include "Server.hpp"
#include "Errors.hpp"

using boost::asio::ip::udp;

int error_handling(int nb_args)
{
    if (nb_args != 3)
        throw ArgumentError("./server <server_ip>");
    return 0;
}

int main(int argc, char** argv)
{
    try {
        error_handling(argc);
        Server server(argv[1], std::atoi(argv[2]));
        return server.run();
    } catch (ArgumentError e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    }
}