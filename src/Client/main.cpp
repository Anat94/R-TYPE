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
#include "Menu.hpp"

using boost::asio::ip::udp;

int main(int argc, char** argv) {
    try {
        if (argc != 3) {
            throw ArgumentError("./client <client_ip> <client_port>");
        }
        enum state _state = MENU;
        while (_state != END) {
            if (_state == MENU) {
                Menu menu;
                _state = menu.run();
            } else if (_state == GAME) {
                Client client(argv[1], atoi(argv[2]));
                return client.run();
            }
            if (_state == SUCCES)
                return 0;
        }
    } catch (ArgumentError e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    } catch (SFMLError e) {
        std::cerr << "Sfml error: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}