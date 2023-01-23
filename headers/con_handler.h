#pragma once
#include <iostream>
#include <boost/asio.hpp>

namespace ba = boost::asio;

class ConHandler : public std::enable_shared_from_this<ConHandler> {
private:
    std::array<ba::ip::tcp::socket, 2> socket_;
    std::array<std::vector<uint8_t>, 2> buffer_;
    ba::ip::tcp::resolver resolver_;
    enum {
        max_length = 1024 * 4
    };
    const std::string &remote_host_;
    const std::string &remote_port_;
    std::size_t request_length_;

    void do_resolve();
    void do_connect(const ba::ip::tcp::endpoint& ep);
    void do_write_request();
    void do_read(int inx);
    void do_write(int inx, std::size_t length);

public:
    explicit ConHandler(ba::ip::tcp::socket socket, ba::io_service &io_service, const std::string &remote_host, const std::string &remote_port);
    void do_read_method();
};