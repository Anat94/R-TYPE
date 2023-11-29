/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"

int main(int argc, char** argv)
{
    Server server(3000, MAX_NB_USER);

    server.startListening();

    return 0;
}
