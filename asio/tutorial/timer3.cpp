// Timer.3 - Bingding arguments to a completion handler 绑定参数到回调函数

#include <functional>
#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code&, boost::asio::steady_timer* t, int* count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);
        // 修改到期时间，原有到期时间+1s
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        // 注册新的定时器
        t->async_wait(std::bind(print, boost::asio::placeholders::error, t, count));
    }
}

int main() {
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    // error是错误码占位符
    t.async_wait(std::bind(print, boost::asio::placeholders::error, &t, &count));
    io.run();
    std::cout << "final count is " << count << std::endl;
    return 0;
}