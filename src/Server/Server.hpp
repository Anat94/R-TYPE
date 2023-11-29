/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>

class Server {
public:
    Server(boost::asio::io_service& io_service, short port);

private:
    void do_accept();
    void handle_client(boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};

#endif // SERVER_HPP
