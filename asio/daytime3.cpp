// Daytime.3 - An asynchronous TCP daytime server.

#include <iostream>
#include <ctime>
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// tcp链接器：socket的封装，用来处理每一个链接请求
// enable_shared_from_this：确保异步操作时，shared_ptr<tcp_connection>对象的存活
class tcp_connection: public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    // 要求使用者必须使用shared_ptr来管理tcp_connection
    // 原因：网络编程有大量的异步操作、异常中断情况，如果手动释放资源，非常复杂易出错
    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket() {
        return socket_;
    }

    void start () {
        message_ = make_daytime_string();
        // 异步回传数据给客户端，并指定下一步操作（数据回传完成或异常中断后立即执行）
        // tcp_connection是用shared_ptr封装的，执行回调函数时，this指针可能不存在，
        // 所以必须传递shared_from_this()延长this生命周期来规避风险
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            std::bind(&tcp_connection::handle_write, shared_from_this(),     
                boost::asio::placeholders::error, // 错误码占位符
                boost::asio::placeholders::bytes_transferred));  // 传输的字节数占位符
    }
private:
    tcp_connection(boost::asio::io_context& io_context): socket_(io_context) {}
    
    // 处理写入后的结果：如处理异常，或记录已传输字节数
    void handle_write(const boost::system::error_code& error, size_t bytes_transferred) {}

    std::string make_daytime_string() {
        std::time_t now = time(0);
        return std::ctime(&now);
    }
private:
    tcp::socket socket_;
    std::string message_;
};

// tcp服务器：用来监听请求，
class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context):
        io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13)) {
            start_accept();
        }
private:
    // 监听客户端请求
    void start_accept() {
        // 本质是shared_ptr指针，原本生命周期到start_accept()函数结束为止
        tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

        // 异步操作，不阻塞
        // 这里使用this，而非shared_from_this是因为创建tcp_server没有使用shared_ptr，且生命周期贯穿整个程序
        // bind()会复制一份参数，以确保执行时参数正确，也就意味着，new_connection的引用计数+1了
        acceptor_.async_accept(new_connection->socket(),
            std::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
    }
    // 处理客户端请求
    // new_connection：是值传递，本身shared_ptr的引用计数也会+1，其生命周期会延长
    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error) {
        if (!error) {
            new_connection->start();
        }
        // 完成处理，继续监听
        start_accept();
    }
private:
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
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