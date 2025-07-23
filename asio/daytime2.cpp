// Daytime.2 - A synchronous TCP daytime server 获取服务器时间的tcp同步服务端

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string() {
    std::time_t now = time(0);
    return std::ctime(&now);
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
        for (;;) {
            tcp::socket socket(io_context);
            // 阻塞当前线程，等待连接
            acceptor.accept(socket);
            // 接收到请求后，返回当前时间
            std::string message = make_daytime_string();
            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}