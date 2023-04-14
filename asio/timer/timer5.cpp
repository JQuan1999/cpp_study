#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>

class printer{
public:
    printer(boost::asio::io_context& io)
        :strand_(boost::asio::make_strand(io)), 
        timer1_(io, boost::asio::chrono::seconds(1)),
        timer2_(io, boost::asio::chrono::seconds(1)), 
        count_(0)
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print1, this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print2, this)));
    }

    void print1(){
        if(count_ < 10){
            std::cout<<"Timer1 : "<<count_<<std::endl;
            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print1, this)));
        }
    }

    void print2(){
        if(count_ < 10){
            std::cout<<"Timer2 : "<<count_<<std::endl;
            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print1, this)));
        }
    }

private:
    // 将调用同一个context_io的run方法多个线程绑定到同一个strand对象上
    // 保证上一个事件处理函数处理完成之前是没法执行下一个事件处理函数, 解决对共享资源访问的同步问题
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};


int main(){
    boost::asio::io_context io;
    printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));// 创建子线程调用io_context.run
    // boost::thread t([&io](){
    //     io.run();
    // });
    io.run();
    t.join();

    return 0;
}