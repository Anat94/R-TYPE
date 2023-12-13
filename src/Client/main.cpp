/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

void send_datas(udp::socket& socket, udp::endpoint& server_endpoint)
{
    std::string message;
    std::getline(std::cin, message);
    socket.send_to(boost::asio::buffer(message), server_endpoint);
}

void receive_datas(udp::socket& socket, udp::endpoint& server_endpoint)
{
    char receive_buffer[1024];
    udp::endpoint sender_endpoint;
    size_t bytes_received = socket.receive_from(boost::asio::buffer(receive_buffer), sender_endpoint);
    receive_buffer[bytes_received] = '\0';
    std::cout << "Received from " << sender_endpoint.address().to_string() << ": " << receive_buffer << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./client <server_ip>" << std::endl;
        return 184;
    }
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));
        udp::endpoint server_endpoint(boost::asio::ip::address::from_string(argv[1]), std::atoi(argv[2]));
        std::cout << "Enter a message to send (Press Ctrl+C to exit):\n";

        while (true) {
            send_datas(socket, server_endpoint);
            receive_datas(socket, server_endpoint);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}