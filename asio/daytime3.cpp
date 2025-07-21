// Daytime.3 - An asynchronous TCP daytime server.

#include <iostream>
#include <boost/asio.hpp>

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context) {}
private:
    
private:
    boost::asio::io_context io_context_;
};

int main () {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}