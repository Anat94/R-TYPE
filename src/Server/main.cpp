/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

void send_datas(udp::socket& socket, udp::endpoint& remote_endpoint)
{
    std::string message;
    std::getline(std::cin, message);
    socket.send_to(boost::asio::buffer(message, message.size()), remote_endpoint);
}

void receive_datas(udp::socket& socket, udp::endpoint& remote_endpoint)
{
    char receive_buffer[1024];
    size_t bytes_received = socket.receive_from(boost::asio::buffer(receive_buffer), remote_endpoint);
    receive_buffer[bytes_received] = '\0';
    std::cout << "Received from " << remote_endpoint.address().to_string() << ": " << receive_buffer << std::endl;
}


int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: ./server <server_ip>" << std::endl;
        return 184;
    }
    try {
        boost::asio::io_context io_context;
        udp::endpoint remote_endpoint(boost::asio::ip::make_address(argv[1]), std::atoi(argv[2]));
        udp::socket socket(io_context, remote_endpoint);
        std::cout << "UDP Server listening on port " << std::atoi(argv[2]) << "...\n";

        while (true) {
            receive_datas(socket, remote_endpoint);
            send_datas(socket, remote_endpoint);

        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}