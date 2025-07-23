// Daytime.5 - A synchronous UDP daytime server 同步循环UDP服务端

#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string() {
    std::time_t now = time(0);
    return std::ctime(&now);
}

int main () {
    try {
        // 1. 创建io上下文
        boost::asio::io_context io_context;
        // 2. 设置监听端点，并创建socket（所有请求共享一个socket）
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));
        for (;;) {
            // 3. 接收请求
            std::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;  // 记录请求端点
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

            // 4. 响应
            std::string message = make_daytime_string();
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
        }

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}