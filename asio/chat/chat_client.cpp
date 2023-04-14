#include<boost/asio.hpp>
#include<iostream>
#include<string>
#include <queue>
#include<thread>
#include "message.h"

using namespace boost::asio::ip;

// 聊天客户端
class Client{
public:
    Client(boost::asio::io_context& ioc, tcp::endpoint& endpoint):_ioc(ioc), _socket(_ioc){
        do_connect(endpoint);
    }

    void close(){
        boost::asio::post(_ioc, [this](){ _socket.close(); });
    }

    void write(const Message msg){
        // The function object is queued for execution
        boost::asio::post(_ioc, [this, msg](){
            // 队列为空时调用do_write
            bool writing = _write_msg_queue.empty();
            _write_msg_queue.push(msg);
            if(writing){
                do_write();
            }
        });
    }
    
private:
    void do_write(){
        // std::cout<<  "thread id:" << std::this_thread::get_id() << std::endl;
        boost::asio::async_write(_socket, boost::asio::buffer(_write_msg_queue.front().Data(), _write_msg_queue.front().Space()), 
        [this](const boost::system::error_code& ec, size_t){
            if(!ec){
                // 发送完毕将队首msg弹出队列
                this->_write_msg_queue.pop();
                // 还有剩余msg未发送继续进行write
                if(!this->_write_msg_queue.empty()){
                    this->do_write();
                }
            }else{
                std::cout<<"write error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
                this->_socket.close();
            }
        });
    }

    void do_connect(tcp::endpoint& endpoint){
        // 异步连接
        _socket.async_connect(endpoint, [this](const boost::system::error_code& ec){
            if(!ec){
                this->do_read();
            }else{
                std::cout<<"connect error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
                this->_socket.close();
            }
        });
    }

    // 读数据
    void do_read(){
        // std::cout<<  "thread id:" << std::this_thread::get_id() << std::endl;
        _socket.async_read_some(boost::asio::buffer(_read_msg.Header(), _read_msg.Space()), 
        [this](const boost::system::error_code& ec, size_t bytes){
            this->read_handler(ec, bytes);
        });
    }

    // 异步读回调函数
    void read_handler(const boost::system::error_code& ec, size_t bytes){
        if(!ec){
            // 写入bytes个字节
            _read_msg.write_n(bytes);
            // std::cout<<"read data: ";
            std::cout.write(_read_msg.Data(), _read_msg.MessageLength());
            _read_msg.Reset();
            // 继续进行异步读数据
            do_read();
        }else if(ec == boost::asio::error::eof)
        {
            std::cout<<"server has closed value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
            _socket.close();
        }else
        {
            std::cout<<"read error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
            _socket.close();
        }
    }

private:
    Message _read_msg;
    std::queue<Message> _write_msg_queue; // 主线程从标准输入读取输入异步发送给服务器 为保证多次异步发送的有序性采用一个队列维护消息
    boost::asio::io_context& _ioc; // 要定义为io_context&
    tcp::socket _socket;
};

int main(int argc, char* argv[]){
    try{
        if(argc != 3){
            std::cerr << "Usage: chat_server <port> <name>\n";
            return 1;
        }
        boost::asio::io_context ioc;
        tcp::endpoint endpoint(address::from_string("127.0.0.1"), atoi(argv[1]));
        Client client(ioc, endpoint);
        // 子线程负责完成读写任务的回调函数
        std::thread th([&ioc](){ 
            // std::cout<<  "thread id:" << std::this_thread::get_id() << std::endl;
            ioc.run(); 
        });
        
        std::string line;
        Message msg(argv[2]);
        while(getline(std::cin, line, '\n')){            
            line += '\n';
            msg.Append(line);
            client.write(msg);
            msg.Reset();
        }
        client.close();
        th.join();

    }catch(std::exception& e){
        std::cerr << "Exception: "<<e.what() << std::endl;
    }
    return 0;
}