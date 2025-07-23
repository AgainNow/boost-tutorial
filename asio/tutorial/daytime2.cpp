// Daytime.2 - A synchronous TCP daytime server 同步循环TCP服务端

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
        // 1. 创建io上下文
        boost::asio::io_context io_context;
        // 2. 设置监听端点
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));
        for (;;) {
            // 3. 创建socket，并开始监听
            tcp::socket socket(io_context);
            // 阻塞当前线程，等待连接
            acceptor.accept(socket);
            // 4. 处理请求
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