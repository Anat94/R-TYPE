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

class UDPServer {
    public:
        UDPServer(boost::asio::io_service& service, unsigned short port)
            : socket_(service, udp::endpoint(udp::v4(), port)) {
            startReceive();
        }

    private:
        void startReceive() {
            socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                [this](boost::system::error_code ec, std::size_t bytes_received) {
                    if (!ec && bytes_received > 0) {
                        std::cout << "Received " << bytes_received << " bytes from: "
                                << remote_endpoint_.address().to_string() << ":" << remote_endpoint_.port() << std::endl;

                        // Process the received data here
                        // Handle the data stored in recv_buffer_

                        startReceive(); // Start listening for more incoming packets
                    } else {
                        std::cerr << "Error receiving data: " << ec.message() << std::endl;
                        startReceive(); // Continue listening for new packets even after an error
                    }
                }
            );
        }
        udp::socket socket_;
        udp::endpoint remote_endpoint_;
        std::array<char, 1024> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_service service;
        // udp::endpoint endpoint(udp::v4(), PORT_NUMBER);
        // udp::socket socket(service, endpoint);
        // std::array<char, 1024> recv_buffer;
        udp::endpoint remote_endpoint;



        // Create a UDP socket
        
        UDPServer server(service, PORT_NUMBER);

        std::cout << "UDP Server listening on port " << PORT_NUMBER << "...\n";
        service.run();


    //     socket.async_receive_from(
    //         boost::asio::buffer(recv_buffer), remote_endpoint,
    //         [this](boost::system::error_code ec, std::size_t bytes_received) {
    //         if (!ec && bytes_received > 0) {
    //             std::cout << "Received " << bytes_received << " bytes from: "
    //                         << remote_endpoint.address().to_string() << ":" << remote_endpoint.port() << std::endl;

    //             startReceive(); // Start listening for more incoming packets
    //         } else {
    //             std::cerr << "Error receiving data: " << ec.message() << std::endl;
    //             startReceive(); // Continue listening for new packets even after an error
    //         }
    //     });
    //     service.run();
    //     while (true) {
    //         // Buffer to store received data
    //         char receive_buffer[1024];
    //         udp::endpoint remote_endpoint;

    //         // Receive data
    //         size_t bytes_received = socket.receive_from(boost::asio::buffer(receive_buffer), remote_endpoint);

    //         // Output received data
    //         std::cout << "Received from " << remote_endpoint.address().to_string() << ": " << receive_buffer << std::endl;

    //         // Echo the received data back to the client
    //         socket.send_to(boost::asio::buffer(receive_buffer, bytes_received), remote_endpoint);
    //     }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}