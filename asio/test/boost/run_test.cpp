#include<thread>
#include<iostream>
#include<boost/asio.hpp>
using namespace boost::asio::ip;

void PrintThreadId()
{
    std::cout<<"thread id: "<<std::this_thread::get_id()<<std::endl;
}

void OnAccept(const boost::system::error_code& ec)
{

}

void OnRead(const boost::system::error_code& ec, size_t bytes)
{
    if(ec){
        std::cout<<"read error, msg: "<<ec.message()<<std::endl;
        PrintThreadId();
    }
    sleep(2);
}

// 测试多线程下测试boost asio的异步回调函数的执行线程
int main()
{
    boost::asio::io_context ioc;
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 12345));
    tcp::socket socket(ioc);
    acceptor.async_accept(socket, std::bind(OnAccept, std::placeholders::_1));
    PrintThreadId();
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    socket.async_read_some(boost::asio::buffer(buf, 1024), std::bind(OnRead, std::placeholders::_1, std::placeholders::_2));
    PrintThreadId();
    ioc.run();
    return 0;
}