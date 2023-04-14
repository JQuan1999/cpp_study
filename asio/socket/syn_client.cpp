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
        // endpoint构造函数
        // 1.传入address和port 例如endpoint(address, port) 用来与remote endpoint连接
        // 2.传入协议和port 例如endpoint(tcp::v4(), port) 通常用来接受一个连接 
        boost::asio::io_context io;
        // 创建套接字
        tcp::socket sock(io);
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(argv[1]), atoi(argv[2])); // 创建endpoint
        boost::system::error_code ec = boost::asio::error::host_not_found;
        // connect连接函数
        sock.connect(endpoint, ec);
        if(ec){
            std::cout<<"connect failed, code is "<<ec.value()<<" error msg is "<<ec.message()<<std::endl;
        }

        for(;;){
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = sock.read_some(boost::asio::buffer(buf), error);

            if(error == boost::asio::error::eof){
                break;
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