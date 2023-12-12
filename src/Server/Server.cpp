/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#include "Server.hpp"
#include <iostream>
#include <thread>

Server::Server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      socket_(io_service) {
    do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept(socket_,
        [this](boost::system::error_code ec) {
            if (!ec) {
                std::thread([this]() {
                    handle_client(std::move(socket_));
                }).detach();
            }

            do_accept();
        }
    );
}

void Server::handle_client(boost::asio::ip::tcp::socket socket) {
    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, '\n');
        std::string data(boost::asio::buffer_cast<const char*>(buffer.data()));
        boost::asio::write(socket, boost::asio::buffer(data));
    } catch (std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}
