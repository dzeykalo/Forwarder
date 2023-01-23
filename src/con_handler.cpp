#include "con_handler.h"

void ConHandler::do_resolve() {
  auto self(shared_from_this());
  resolver_.async_resolve({ remote_host_, remote_port_ },
                          [this, self](const boost::system::error_code& ec, const ba::ip::tcp::resolver::iterator& it) {
                              if (!ec) {
                                do_connect(it->endpoint());
                              } else {
                                std::cerr << ec.message();
                              }
                          });
}

void ConHandler::do_connect(const ba::ip::tcp::endpoint& ep) {
  auto self(shared_from_this());
  socket_[1].async_connect(ep,
                           [this, self](const boost::system::error_code& ec) {
                               if (!ec) {
                                 std::cout << "connected to " << remote_host_ << ":" << remote_port_;
                                 do_write_request();
                               } else {
                                 std::cerr << ec.message();
                               }
                           });

}

void ConHandler::do_write_request() {
  auto self(shared_from_this());
  socket_[1].async_write_some(ba::buffer(buffer_[0], request_length_),
                              [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                  if (!ec) {
                                    do_read(0);
                                    do_read(1);
                                  } else {
                                    std::cerr << ec.message();
                                    socket_[0].close();
                                  }
                              });
}

void ConHandler::do_read(int inx) {
  auto self(shared_from_this());
  socket_[inx].async_read_some(ba::buffer(buffer_[inx], max_length),
                               [this, self, inx](boost::system::error_code ec, std::size_t length) {
                                   if (!ec) {
                                     do_write(inx ^ 1, length);
                                   }
                               });
}

void ConHandler::do_write(int inx, std::size_t length) {
  auto self(shared_from_this());
  socket_[inx].async_write_some(ba::buffer(buffer_[inx^1], length),
                                [this, self, inx](boost::system::error_code ec, std::size_t /*length*/) {
                                    if (!ec) {
                                      do_read(inx^1);
                                    }
                                });
}

ConHandler::ConHandler(ba::ip::tcp::socket socket, ba::io_service &io_service, const std::string &remote_host, const std::string &remote_port) :
    socket_{std::move(socket), ba::ip::tcp::socket(io_service)},
    buffer_{std::vector<uint8_t>(max_length, '\0'), std::vector<uint8_t>(max_length, '\0')}, resolver_(io_service),
    remote_host_(remote_host), remote_port_(remote_port) {}

void ConHandler::do_read_method() {
  auto self(shared_from_this());
  socket_[0].async_read_some(ba::buffer(buffer_[0], max_length),
                             [this, self](boost::system::error_code ec, std::size_t length) {
                                 if (!ec) {
                                   request_length_ = length;
                                   do_resolve();
                                 } else {
                                   std::cerr << ec.message();
                                 }
                             });
}
