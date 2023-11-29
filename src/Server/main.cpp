/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <cstdio>

int main() {

    Server server(3000, MAX_NB_USER);

    server.startListening();
    return 0;
}