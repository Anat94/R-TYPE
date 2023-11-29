/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp" 
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int _port, int _max_clients) : port(_port), max_clients(_max_clients) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Erreur lors de la création du socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Erreur lors du bind" << std::endl;
        close(serverSocket);
        return;
    }
    if (listen(serverSocket, max_clients) == -1) {
        perror("SERVER ERROR");
        return;
    }
    FD_ZERO(&read_fds);
    FD_SET(serverSocket, &read_fds);
}

Server::~Server() {
    close(serverSocket);
    for (int clientSocket : clientSockets) {
        close(clientSocket);
    }
}

int Server::startListening() {
    printf("[SERVER] starting port %i\n", port);

    FD_ZERO(&read_fds);
    FD_SET(serverSocket, &read_fds);
    for (int client_connected : clientSockets) {
        FD_SET(client_connected, &read_fds);
    }
    
    while (1) {
        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
            perror("SERVER SELECT ERROR");
            return 1;
        }
        handle_select();
        FD_ZERO(&read_fds);
        FD_SET(serverSocket, &read_fds);
        for (int client_connected : clientSockets) {
            FD_SET(client_connected, &read_fds);
        }
    }

    return 0;
}

bool Server::handle_select()
{
    for (int i = 0; i < FD_SETSIZE; i++) {
        if (FD_ISSET(i, &read_fds) && i == serverSocket) {
            accept_new_client();
            return true;
        }
        if (FD_ISSET(i, &read_fds) && is_client_connected(i)) {
            //parse_command_for_client(server, i);
            return true;
        }
        return false;
    }
}

bool Server::is_client_connected(int client_socket)
{
    for (int client_connected : clientSockets) {
        if (client_connected == client_socket)
            return true;
    }
    return false;
}

void Server::accept_new_client()
{
    std::string message = "Welcome\n";

    int new_client = accept(serverSocket, nullptr, nullptr);
    clientSockets.push_back(new_client);
    send(clientSockets.back(), message.c_str(), message.size(), 0);
}
