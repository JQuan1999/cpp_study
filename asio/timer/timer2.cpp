#include<iostream>
#include<boost/asio.hpp>

void print(const boost::system::error_code&)
{
    std::cout << "Hello, world!" << std::endl;
}

int main()
{
    boost::asio::io_context io;
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(2));
    // 调用steady_timer::async_wait()异步等待
    t.async_wait(&print);
    std::cout<<"in main function"<<std::endl;
    sleep(5);
    std::cout<<"sleep over"<<std::endl;
    io.run(); // 调用io_context::run的线程将调用print回调函数
    return 0;
}