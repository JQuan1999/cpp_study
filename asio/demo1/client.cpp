#include<iostream>
#include<boost/asio.hpp>

using namespace boost::asio::ip;

/*
    同步连接应答式客户端 客户端发送一条消息 服务端将消息返回
*/
const int MAX_LENGTH = 1024;
int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Usage: client <ip> <port>"<< std::endl;
        return 1;
    }
    try{
        // 创建上下文服务
        boost::asio::io_context ioc;
        tcp::socket sock(ioc);
        // 构造endpoint
        tcp::endpoint ep(address::from_string(argv[1]), atoi(argv[2]));
        boost::system::error_code ec = boost::asio::error::host_not_found;
        sock.connect(ep, ec);
        if(ec){
            std::cout << "connect failed, code is " << ec.value() << " error msg is " << ec.message() << std::endl;
			return 0;
        }
        char request[MAX_LENGTH];
        char recieve[MAX_LENGTH];
        for(; ;){
            memset(request, '\0', MAX_LENGTH);
            memset(recieve, '\0', MAX_LENGTH);
            std::cout << "Enter message: ";
            std::cin.getline(request, MAX_LENGTH, '\n');

            size_t request_length = strlen(request);
            boost::asio::write(sock, boost::asio::buffer(request, request_length), ec);
            if(ec){
                std::cout << "connect failed, code is " << ec.value() << " error msg is " << ec.message() << std::endl;
                break;
            }else if(ec){
                throw boost::system::system_error(ec);
            }
            // 该函数调用将阻塞，直到一个或多个字节的数据被成功读取，或者直到发生错误
            size_t recieve_length = sock.read_some(boost::asio::buffer(recieve, MAX_LENGTH), ec);
            if(ec){
                std::cout << "connect failed, code is " << ec.value() << " error msg is " << ec.message() << std::endl;
                break;
            }
            std::cout<<"Reply is: ";
            std::cout.write(recieve, recieve_length);
            std::cout<<std::endl;
        }
    }
    catch(std::exception& e){
        std::cerr << "Exception: "<<e.what()<<std::endl;
    }
    return 0;
}
