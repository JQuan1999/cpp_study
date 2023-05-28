#include<boost/asio.hpp>
#include<iostream>
using namespace boost::asio::ip;

void on_connect(const boost::system::error_code& ec)
{   
    sleep(10);
    if(ec){
        std::cout<<"connec error, error msg: "<<ec.message()<<std::endl;
    }else{
        std::cout<<"connec successful"<<std::endl;
    }
}

int main()
{
    boost::asio::io_context ioc;
    tcp::endpoint endpoint(address::from_string("127.0.0.1"), 12345);
    tcp::socket socket(ioc);
    socket.async_connect(endpoint, std::bind(&on_connect, std::placeholders::_1));
    ioc.run();
    return 0;
}