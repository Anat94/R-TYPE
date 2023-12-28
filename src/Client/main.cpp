/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <asio.hpp>
#include "../Errors.hpp"
#include "Client.hpp"
#include "Menu.hpp"

using asio::ip::udp;

int main(int argc, char** argv) {
    std::string tmp_username;

    try {
        if (argc != 3) {
            throw ArgumentError("./client <client_ip> <client_port>");
        }
        enum state _state = MENU;
        while (_state != END) {
            if (_state == MENU) {
                Menu menu;
                _state = menu.run();
                tmp_username = menu.getUsername();
                std::cout << "username: " << tmp_username << std::endl;
            } else if (_state == GAME) {
                Client client(argv[1], atoi(argv[2]), tmp_username);
                return client.run();
            }
            if (_state == SUCCES)
                return 0;
        }
    } catch (const ArgumentError &e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    } catch (const SFMLError &e) {
        std::cerr << "Sfml error: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
