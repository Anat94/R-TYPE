/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef MAIN_HPP_
    #define MAIN_HPP_
    #define MAX_NB_USER 1000
    #include <netinet/in.h>
    #include <vector>

class Server {
    public:
        Server(int port, int max_clients);
        ~Server();
        int startListening();
    private:
        int port;
        int serverSocket;
        fd_set read_fds;
        int max_clients;
        struct sockaddr_in serverAddr;
        std::vector<int> clientSockets;

        bool handle_select();
        bool is_client_connected(int client_socket);
        void accept_new_client();
};

#endif /* !MAIN_HPP_ */
