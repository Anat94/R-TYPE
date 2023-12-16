/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <iostream>
#include <thread>

Server::Server(boost::asio::io_service &service, int port): _service(service), _socket(service, udp::endpoint(udp::v4(), port))
{
    _tpool.emplace_back([this, &service]() { service.run(); });
    receive_datas();
}

Server::~Server() {}

// void Server::send_datas()
// {
//     std::string message;
//     std::getline(std::cin, message);
//     _socket.send_to(boost::asio::buffer(message, message.size()), _remote_endpoint);
// }

void Server::receive_datas()
{
    _socket.async_receive_from(
        boost::asio::buffer(_buf), _remote_endpoint,
        [this](const std::error_code& ec, std::size_t bytes_recvd) {
            if (!ec) {
                std::cout << "Received: " << std::string(_buf.begin(), _buf.begin() + bytes_recvd) << std::endl;
                receive_datas(); // Continue listening for incoming data
            } else {
                std::cerr << "Error: " << ec.message() << std::endl;
            }
        }
    );
}

// int Server::run() {
//     while (true) {
//         receive_datas();
//         send_datas();
//     }
//     return 0;
// }

// Server::Server(boost::asio::io_service& io_service, short port)
//     : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
//       socket_(io_service) {
//     do_accept();
// }

// void Server::do_accept() {
//     acceptor_.async_accept(socket_,
//         [this](boost::system::error_code ec) {
//             if (!ec) {
//                 std::thread([this]() {
//                     handle_client(std::move(socket_));
//                 }).detach();
//             }

//             do_accept();
//         }
//     );
// }

// void Server::handle_client(boost::asio::ip::tcp::socket socket) {
//     try {
//         boost::asio::streambuf buffer;
//         boost::asio::read_until(socket, buffer, '\n');
//         std::string data(boost::asio::buffer_cast<const char*>(buffer.data()));
//         boost::asio::write(socket, boost::asio::buffer(data));
//     } catch (std::exception& e) {
//         std::cerr << "Exception in thread: " << e.what() << std::endl;
//     }
// }
