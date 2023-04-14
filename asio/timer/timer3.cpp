#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// 实现使用asio实现可重复print
// 为了实现重复print, 处理程序需要能够访问定时器对象 添加了两个函数参数
void print(const boost::system::error_code&, boost::asio::steady_timer* t, int* count)
{
    if(*count < 5){
        std::cout<<*count<<std::endl;
        ++(*count);
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
}

int main()
{
    boost::asio::io_context io;
    int count = 0;
    boost::asio::steady_timer t(io,boost::asio::chrono::seconds(1));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));
    io.run();
    std::cout<<"Final count is"<<count<<std::endl;
    return 0;
}