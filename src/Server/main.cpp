/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int PORT_NUMBER = 8888;

int main()
{
    try {
        boost::asio::io_context io_context;
        udp::endpoint endpoint(boost::asio::ip::make_address("127.0.0.1"), PORT_NUMBER);
        udp::socket socket(io_context, endpoint);
        std::cout << "UDP Server listening on port " << PORT_NUMBER << "...\n";

        while (true) {
            char receive_buffer[1024];
            udp::endpoint remote_endpoint;

            size_t bytes_received = socket.receive_from(boost::asio::buffer(receive_buffer), remote_endpoint);
            receive_buffer[bytes_received] = '\0';

            std::cout << "Received from " << remote_endpoint.address().to_string() << ": " << receive_buffer << std::endl;

            std::string message;
            std::getline(std::cin, message);
            socket.send_to(boost::asio::buffer(message, message.size()), remote_endpoint);
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}