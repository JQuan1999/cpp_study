#include<iostream>
#include<boost/asio.hpp>
#include<memory>
#include<set>

using namespace boost::asio::ip;
using sock_ptr = std::shared_ptr<tcp::socket>;
using thread_ptr = std::shared_ptr<std::thread>;

std::set<thread_ptr> thread_set;

const int MAX_LENGTH = 1024;

void session(sock_ptr sock){
    try{
        char recieve[MAX_LENGTH];
        for(; ;){
            memset(recieve, 0, MAX_LENGTH);
            boost::system::error_code ec;
            size_t length = sock->read_some(boost::asio::buffer(recieve, MAX_LENGTH), ec);
            if(ec == boost::asio::error::eof){
                std::cout<<"connection closed by user "<<std::endl;
                break;
            }else if(ec){
                throw boost::system::system_error(ec); // 抛出异常
            }
            std::cout << "recieve from "<<sock->remote_endpoint().address().to_string()<<std::endl;
            std::cout << "recieve msg is "<<recieve<< std::endl;
            // 回复信息
            boost::asio::write(*sock, boost::asio::buffer(recieve, length));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Exception: "<<e.what()<<std::endl;
    }
}

void server(boost::asio::io_context& ioc, unsigned short port){
    tcp::acceptor acp(ioc, tcp::endpoint(tcp::v4(), port));
    for(;;){
        // 创建接受连接的sock, 动态分配内存防止离开作用域被析构
        sock_ptr sock(new tcp::socket(ioc));
        acp.accept(*sock);
        // 创建子线程指针 调用session函数
        thread_ptr t = std::make_shared<std::thread>(session, sock);
        thread_set.insert(t);
    }
}

/*
    同步连接应答式客户端 客户端发送一条消息 服务端将消息返回
    服务端对每个接受到的客户端连接都新建一个线程进行处理(优化 采用线程池)
*/
int main(){
    try{
        boost::asio::io_context ioc;
        server(ioc, 12345);
        for(auto& t: thread_set){
            t->join(); // 等待所有线程完成任务后退出
        }
    }
    catch(const std::exception& e){
        std::cerr << "Exception: "<<e.what()<<std::endl;
    }
    return 0;
}