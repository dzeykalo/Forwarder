#include "server.h"
#include "con_handler.h"

void Server::do_accept() {
  acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
      if (!ec) {
        boost::asio::post(pool_, [this](){
          std::make_shared<ConHandler>(std::move(socket_), io_service_, remote_host_, remote_port_)->do_read_method();
        });
      }
      do_accept();
  });
}

Server::Server(ba::io_service &io_service, const ba::ip::tcp::endpoint &endpoint, std::string &&remote_host, std::string &&remote_port) :
    socket_(io_service), acceptor_(io_service, endpoint), io_service_(io_service), remote_host_(remote_host), remote_port_(remote_port),
    pool_(4) {
  do_accept();
}
