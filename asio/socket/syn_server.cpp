#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    time_t now = time(0);
    return ctime(&now);
}

int main(){
    try{
        boost::asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345)); // tcp::v4()返回本机地址

        for(; ;){
            tcp::socket sock(io);
            acceptor.accept(sock);
            std::string message = make_daytime_string();

            boost::system::error_code ingored_error;
            boost::asio::write(sock, boost::asio::buffer(message), ingored_error);
            
        }
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}