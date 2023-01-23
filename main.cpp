#include <boost/asio.hpp>
#include <iostream>
#include "server.h"
#include "utils.hpp"

namespace ba = boost::asio;

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "Usage: fw <local_port:remote_host:remote_port>\n";
            return 1;
        }
        auto [local_port, remote_host, remote_port] = get_options(argv[1]);
        if (local_port == 0) {
          std::cerr << "Usage: fw <local_port:remote_host:remote_port>\n";
          return 1;
        }

        boost::asio::io_service io_service;
        ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), local_port);
        Server server(io_service, endpoint, std::move(remote_host), std::move(remote_port));
        io_service.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
