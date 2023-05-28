#include<boost/asio.hpp>
#include<iostream>
using namespace boost::asio::ip;
int main()
{
    boost::asio::io_context ioc;
    tcp::resolver solver(ioc);
    tcp::resolver::iterator iter = solver.resolve(tcp::resolver::query("www.baidu.com", std::to_string(80))), end;
    for(; iter != end; iter++){
        std::cout<<iter->endpoint()<<std::endl;
    }
    return 0;
}