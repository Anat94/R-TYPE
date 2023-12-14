/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>
#include "../Errors.hpp"
#include "Client.hpp"

using boost::asio::ip::udp;

int main(int argc, char** argv) {
    try {
        if (argc != 3) {
            throw ArgumentError("./client <client_ip> <client_port>");
        }
        Client client(argv[1], atoi(argv[2]));
        return client.run();
    } catch (ArgumentError e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}