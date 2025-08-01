// Daytime.1 - A synchronous TCP daytime client 同步循环TCP客户端
#include <array>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: client " << std::endl;
            return 1;
        }
        // 1. 创建io上下文
        boost::asio::io_context io_context;
        
        // 2. 设置目标端点
        // 解析器，将域名转化为端点列表（即ip+port）
        tcp::resolver resolver(io_context);
        // 将域名:服务名转化为端点列表
        // param1: 域名或ip
        // param2: 服务名或端口号，常用服务都有专属端口号，如daytime=13，http=80
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
        // 3. 创建socket，申请连接
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);
        // 4. 循环发送请求并处理响应
        for (;;) {
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);
            
            std::cout.write(buf.data(), len);
        } 
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}