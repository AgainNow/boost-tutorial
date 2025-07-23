// Daytime.6 - An asynchronous UDP daytime server 异步循环UDP服务端（同一时间只能处理一个请求）

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class udp_server {
public:
    udp_server(boost::asio::io_context& io_context):
        socket_(io_context, udp::endpoint(udp::v4(), 13)) {
        start_receive();
    }
private:
    // 接收请求
    void start_receive() {
        // 在事件队列中注册一个回调函数handle_receive来处理接收到的请求，触发条件是接收到请求
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
            std::bind(&udp_server::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
 
    // 处理请求
    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
        if (!error) {
            std::shared_ptr<std::string> message(new std::string(make_daytime_string()));
            socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                std::bind(&udp_server::handle_send, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            start_receive();  // 处理完上一个请求，才会重新启动监听
        }
    }
 
    // 处理响应结果：处理异常或记录传输的字节数
    void handle_send(std::shared_ptr<std::string> message, 
        const boost::system::error_code& error, 
        std::size_t bytes_transferred) {}
 
    std::string make_daytime_string() {
        std::time_t now = time(0);
        return std::ctime(&now);
    }
private:
    udp::socket socket_;
    std::array<char, 1> recv_buffer_;
    udp::endpoint remote_endpoint_;  // 同一时间只能处理一个请求，所以此处没有同步问题
};

int main() {
    try {
        boost::asio::io_context io_context;
        udp_server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}