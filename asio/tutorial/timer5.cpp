// Timer.5 - Synchronising completion handlers in multithreaded programs

#include <functional>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>

class printer {
public:
    printer(boost::asio::io_context& io): 
        strand_(boost::asio::make_strand(io)),
        timer1_(io, boost::asio::chrono::seconds(1)),
        timer2_(io, boost::asio::chrono::seconds(1)),
        count_(0) {
            // 将回调函数与执行上下文绑定
            timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print1, this)));
            timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print2, this)));
        }
    ~printer() {
        std::cout << "final count is " << count_ << std::endl;
    }

    void print1() {
        if (count_ < 10) {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print1, this)));
        }
    }

    void print2() {
        if (count_ < 10) {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
            timer2_.async_wait(boost::asio::bind_executor(strand_, std::bind(&printer::print2, this)));
        }
    }
private:
    // 执行上下文，用于处理线程同步问题，同一个strand中的回调函数是顺序执行的
    // 执行顺序取决于回调函数注册时间、定时器的精确触发时间、操作系统的调度（线程获取时间片的时间）
    // 所以：print1和print2不一定是交替执行的，执行次数也不一定相同
    // executor是执行上下文的抽象，定义了如何执行异步操作（如回调函数），包括在哪个线程、以何种顺序执行
    // io_context::executor_type是io_context默认的executor类型，表示异步操作将在该io_context所属的线程池中执行
    // strand<io_context::executor_type>是一种特化的executor，增加了顺序执行的要求
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

int main() {
    boost::asio::io_context io;
    printer p(io);
    // 两个线程并发执行所有异步操作（两个线程并发执行事件循环，共同处理所有回调函数）
    std::thread t([&]{ io.run(); });
    io.run();

    t.join();

    return 0;
}