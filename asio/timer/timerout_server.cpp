#include<boost/asio.hpp>
#include<iostream>
using namespace boost::asio::ip;

enum STATE{
    CLOSED = 1,
    CONNECTED = 2
};
boost::asio::io_context ioc;
tcp::socket remote(ioc);
STATE state = CLOSED;

void on_accept(const boost::system::error_code& ec){
    if(ec){
        std::cout<<"accept error, error msg: "<<ec.message()<<std::endl;
        state = CLOSED;
        return;
    }else{
        state = CONNECTED;
        std::cout<<"accept sucessful "<<std::endl;
        return;
    }
}


void on_wait(const boost::system::error_code& ec)
{
    if(ec){
        std::cout<<"on wait error, error msg: "<<ec.message()<<std::endl;
        return;
    }
    if(state != CONNECTED){
        state = CLOSED;
        remote.close();
        std::cout<<"connection time out "<<std::endl;
    }
}


int main()
{
    tcp::endpoint endpoint(tcp::v4(), 12345);
    tcp::acceptor acceptor(ioc, endpoint);
    acceptor.async_accept(remote, std::bind(&on_accept, std::placeholders::_1));
    boost::asio::steady_timer timer(ioc, boost::asio::chrono::seconds(5));
    timer.async_wait(std::bind(&on_wait, std::placeholders::_1));
    ioc.run();
    if(state == CLOSED){
        std::cout<<"socket is closed"<<std::endl;
    }else{
        std::cout<<"socket is connected"<<std::endl;
    }
    return 0;
}