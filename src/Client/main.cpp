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

using asio::ip::udp;

/**
 * @brief main function
 *
 * @param argc number of arguments
 * @param argv tab of arguments
 * @return int  0 if success
 *              84 if error
 */
int main(int argc, char** argv) {
    std::string tmp_username;
    registry ecs;
    std::mutex mtx;
    EventListener listener;

    ecs.mtx = &mtx;
    listener.addRegistry(ecs);

    try {
        if (argc != 3) {
            throw ArgumentError("./client <client_ip> <client_port>");
        }
        Client client(argv[1], atoi(argv[2]), listener, ecs, mtx);
        return client.run();
    } catch (const ArgumentError &e) {
        std::cerr << "Usage: " << e.what() << std::endl;
        return 84;
    } catch (const SFMLError &e) {
        std::cerr << "Sfml error: " << e.what() << std::endl;
        return 84;
    }

    return 0;
}
