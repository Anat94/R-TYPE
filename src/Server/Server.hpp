/*
** EPITECH PROJECT, 2023
** rtype
** File description:
** main
*/

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <boost/asio.hpp>


using boost::asio::ip::udp;


class Server {
    public:
        Server(std::string ip, int port);
        ~Server();
        int run();
        void send_datas();
        void receive_datas();

    private:
        boost::asio::io_context _io_context;
        udp::endpoint _remote_endpoint;
        udp::socket _socket;
};

// class tcp_connection {
//     public:
//         tcp_connection(boost::asio::io_service& io_service) : m_socket(io_service) {}

//         boost::asio::ip::tcp::socket& socket() { return m_socket; }

//         template <typename T, typename Handler>
//         void async_write(const T& t, Handler handler) {
//             std::ostringstream archive_stream;
//             boost::archive::text_oarchive archive(archive_stream);
//             archive << t;
//             m_outbound_data = archive_stream.str();
// --
//             std::ostringstream header_stream;
//             header_stream << std::setw(header_length)
//                 << std::hex << m_outbound_data.size();
//             if (!header_stream || header_stream.str().size() != header_length) {
//                 boost::system::error_code error(boost::asio::error::invalid_argument);
//                 m_socket.io_service().post(boost::bind(handler, error));
//                 return;
//             }
//             m_outbound_header = header_stream.str();

//             std::vector<boost::asio::const_buffer> buffers;
//             buffers.push_back(boost::asio::buffer(m_outbound_header));
//             buffers.push_back(boost::asio::buffer(m_outbound_data));
//             boost::asio::async_write(m_socket, buffers, handler);
//         }

//         template <typename T, typename Handler>
//         void async_read(T& t, Handler handler) {
//             void (tcp_connection::*f)( const boost::system::error_code&, T&, boost::tuple<Handler>)
//                 = &tcp_connection::handle_read_header<T, Handler>;
//             boost::asio::async_read(m_socket, boost::asio::buffer(m_inbound_header),
//                 boost::bind(f,
//                 this, boost::asio::placeholders::error, boost::ref(t),
//                 boost::make_tuple(handler)));
//         }	

//         template <typename T, typename Handler>
//         void handle_read_header(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler) {
//             if (e)
//                 boost::get<0>(handler)(e);
//             else {
//                 std::istringstream is(std::string(m_inbound_header, header_length));
//                 std::size_t m_inbound_datasize = 0;
//                 if (!(is >> std::hex >> m_inbound_datasize)) {
//                     boost::system::error_code error(boost::asio::error::invalid_argument);
//                     boost::get<0>(handler)(error);
//                     return;
//                 }

//                 m_inbound_data.resize(m_inbound_datasize);
//                 void (tcp_connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>) = &tcp_connection::handle_read_data<T, Handler>;
                    
//                 boost::asio::async_read(m_socket, boost::asio::buffer(m_inbound_data), boost::bind(f, this, boost::asio::placeholders::error, boost::ref(t), handler));
//             }
//         }

//         template <typename T, typename Handler>
//         void handle_read_data(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler) {
//             if (e)
//                 boost::get<0>(handler)(e);
//             else {
//                 try {
//                     std::string archive_data(&m_inbound_data[0], m_inbound_data.size());
//                     std::istringstream archive_stream(archive_data);
//                     boost::archive::text_iarchive archive(archive_stream);
//                     archive >> t;
//                 }
//                 catch (std::exception& e) {
//                     boost::system::error_code error(boost::asio::error::invalid_argument);
//                     boost::get<0>(handler)(error);
//                     return;
//                 }
//                 boost::get<0>(handler)(e);
//             }
//         }

//     private:
//         boost::asio::ip::tcp::socket m_socket;
//         enum { header_length = 8 };
//         std::string m_outbound_header;
//         std::string m_outbound_data;
//         char m_inbound_header[header_length];
//         std::vector<char> m_inbound_data;
// };

// typedef boost::shared_ptr<tcp_connection> connection_ptr;

#endif // SERVER_HPP
