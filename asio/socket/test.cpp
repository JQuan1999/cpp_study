#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::array;
boost::asio::io_service service;
tcp::resolver resolver(service);
tcp::socket sock(service);
array<char, 4096> buffer;

void read_handler(const boost::system::error_code &ec, std::size_t transferred_byte)
{
	if (!ec)
	{
		std::cout << std::string(buffer.data(), transferred_byte) << std::endl;
		sock.async_read_some(boost::asio::buffer(buffer), read_handler);
	}
}


void connect_handler(const boost::system::error_code& ec){
    if(!ec){
        boost::asio::write(sock, boost::asio::buffer("GET / HTTP 1.1\r\nHost: www.baidu.com\r\n\r\n"));
		sock.async_read_some(boost::asio::buffer(buffer), read_handler);
    }
}


void resolve_handler(const boost::system::error_code& ec, tcp::resolver::iterator it){
    if(!ec){
        sock.async_connect(*it, connect_handler);
    }
}

int main()
{
    tcp::resolver::query query("www.baidu.com", "80");
    resolver.async_resolve(query, resolve_handler);
    service.run();
    system("pause");
    return 0;
}