/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/
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
        enum state state = MENU;
        while (state != END) {
            if (state == MENU) {
                Menu menu;
                state = menu.run();
                tmp_username = menu.getUsername();
                std::cout << "username: " << tmp_username << std::endl;
            } else if (state == GAME) {
                Client client(argv[1], atoi(argv[2]), tmp_username);
                return client.run();
            }
            if (state == SUCCES)
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
