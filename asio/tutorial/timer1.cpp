// Timer.1 - Using a timer synchronously 同步定时器
// link: https://www.boost.org/doc/libs/latest/doc/html/boost_asio/tutorial/tuttimer1.html
#include <iostream>
#include <boost/asio.hpp>

int main () {
    // 只要使用asio，至少有一个io_context
    boost::asio::io_context io;
    // 定时器，创建时即开始计时
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    // 阻塞当前线程，直到定时器到期
    t.wait();
    std::cout << "hello world" << std::endl;
    return 0;
}