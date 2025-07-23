// Daytime.7 - A combined TCP/UDP asynchronous server 同时支持TCP和UDP的异步服务端（示例不参与编译）

#include <array>
#include <iostream>
#include <boost/asio.hpp>
 
class tcp_server;  // 和daytime3一致
class udp_server;  // 和daytime6一致

int main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server1(io_context);
        udp_server server2(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}