/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

const int SERVER_PORT = 8888;

int main() {
    try {
        boost::asio::io_context io_context;

        // Create a UDP socket
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0));

        // Server endpoint
        udp::endpoint server_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), SERVER_PORT);

        std::cout << "Enter a message to send (Press Ctrl+C to exit):\n";

        while (true) {
            std::string message;
            std::getline(std::cin, message);

            // Send data to the server
            socket.send_to(boost::asio::buffer(message), server_endpoint);

            // Buffer to receive data
            char receive_buffer[1024];
            udp::endpoint sender_endpoint;

            // Receive data from the server
            size_t bytes_received = socket.receive_from(boost::asio::buffer(receive_buffer), sender_endpoint);

            // Output received data
            std::cout << "Received from " << sender_endpoint.address().to_string() << ": " << receive_buffer << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}