#pragma once
#include <boost/asio.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

namespace ba = boost::asio;

class Server {
private:
    ba::ip::tcp::socket socket_;
    ba::ip::tcp::acceptor acceptor_;
    ba::io_service &io_service_;
    std::string remote_host_;
    std::string remote_port_;
    ba::thread_pool pool_;
    void do_accept();

public:
    Server(ba::io_service &io_service, const ba::ip::tcp::endpoint &endpoint, std::string &&remote_host, std::string &&remote_port);
};
