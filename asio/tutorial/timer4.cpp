// Timer.4 - Using a member function as a completion handler 类成员函数作为回调函数

#include <functional>
#include <iostream>
#include <boost/asio.hpp>

class printer {
public:
    printer(boost::asio::io_context& io): 
        timer_(io, boost::asio::chrono::seconds(1)), 
        count_(0) {
        // 类成员函数都有一个默认的this参数
        timer_.async_wait(std::bind(&printer::print, this));
    }
    ~printer() {
        std::cout << "final count is " << count_ << std::endl;
    }

public:
    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(std::bind(&printer::print, this));
        }
    }

private:
    boost::asio::steady_timer timer_;
    int count_;
};


int main() {
    boost::asio::io_context io;
    printer p(io);
    io.run();

    return 0;
}