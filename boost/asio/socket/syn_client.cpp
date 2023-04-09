#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;

// int main()
int main(int argc, char* argv[]){
    try{
        if(argc != 3){
            std::cerr << "Usage: client <ip> <port>"<< std::endl;
            return 1;
        }
        boost::asio::io_context io;
        
        
        // boost::asio::ip::address add;
        // add.from_string("127.0.0.1");
        // tcp::endpoint endpoints(add, short(12345));
        
        tcp::resolver resolver(io);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator endpoint_iterator =  resolver.resolve(query);
        // tcp::endpoint endpoint = *endpoint_iterator;
        
        // tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");

        // 创建套接字
        tcp::socket sock(io);
        boost::asio::connect(sock, endpoint_iterator);

        for(;;){
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = sock.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof){
                continue;
            }else if(error)
                throw boost::system::system_error(error); // some other error

            std::cout.write(buf.data(), len);
        }
    }
    catch(std::exception& e){
        std::cerr<<e.what()<<std::endl;
    }
    
    return 0;
}