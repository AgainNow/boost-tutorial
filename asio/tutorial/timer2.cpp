// Timer.2 - Using a timer asynchronously 异步定时器

#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code&) {
    std::cout << "hello world" << std::endl;
}

int main () {
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
    // 异步等待，注册回调函数print
    // 不会阻塞当前线程；不会创建新线程
    // 到期后，print会在io_context所在的任一空闲线程执行
    t.async_wait(&print);
    // 启动事件循环，阻塞当前线程，直到所有异步工作完成为止；若漏写，回调函数永远不会执行
    io.run();
    return 0;
}