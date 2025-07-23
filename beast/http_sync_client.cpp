// Simple HTTP Client
// Use HTTP to make a GET request to a website and print the reponse:
// File: http_sync_client.cpp

#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/beast.hpp>

using boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace http = boost::beast::http;

int main (int argc, char* argv[]) {
    try {
        if (argc != 4 && argc != 5) {
            std::cerr <<
                "Usage: http-client-sync <host> <port> [<HTTP version: 1.0 or 1.1(default)>\n]" <<
                "Example:\n" <<
                "   http-client-sync www.example.com 80 /\n"  <<
                "   http-client-sync www.example.com 80 / 1.0\n";
            return EXIT_FAILURE;
        }
        auto const host = argv[1];
        auto const port = argv[2];
        auto const target = argv[3];
        int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

        // 创建io上下文
        boost::asio::io_context ioc;
        // 解析端点，并连接
        tcp::resolver resolver(ioc);
        boost::beast::tcp_stream stream(ioc);
        auto const results = resolver.resolve(host, port);
        stream.connect(results);

        // 构造http请求
        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // 发送请求
        http::write(stream, req);

        // 接收响应并处理
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        http::read(stream, buffer, res);
        std::cout << res << std::endl;

        // 关闭socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        
        // 处理异常
        if (ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}