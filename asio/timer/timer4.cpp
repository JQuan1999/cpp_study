#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// 将一个成员函数作为回调函数
class printer
{
public:
    explicit printer(boost::asio::io_context& io): timer_(io, boost::asio::chrono::seconds(1)), count_(0){
        timer_.async_wait(boost::bind(&printer::print, this));
    }
    void print(){
        if(count_ < 5){
            std::cout<<count_<<std::endl;
            ++count_;
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1)); // 延长1秒
            timer_.async_wait(boost::bind(&printer::print, this)); // 再次绑定
        }
    }
private:
    boost::asio::steady_timer timer_;
    int count_;
};

int main()
{
    boost::asio::io_context io;
    printer pt(io);
    io.run();
    return 0;
}