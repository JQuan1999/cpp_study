#include<iostream>
#include<boost/asio.hpp>
#include<time.h>
#include<thread>
#include<string>
#include<vector>
using namespace boost::asio::ip;

/*
    测试tcp粘包处理 客户端发送数据 服务端2s接收一次将粘包的数据进行拆分
    自定义消息格式：消息头部(4byte)+消息体
*/
const uint32_t DATA_SIZE = 4;
const uint32_t DATA_LENGTH = 128;
const uint32_t MESSAGE_LENGTH = DATA_SIZE + DATA_LENGTH;

const int MAX_WRITE_COUNT = 50;
std::vector<int> sends;
std::vector<int> reads;

std::string GenerateMessage()
{
    uint32_t size = rand() % DATA_LENGTH;
    std::string msg;
    for(uint32_t i = 0; i < size; i++)
    {
        int p = rand() % 26;
        msg += char('a' + p);
    }
    std::cout<<"next send data: "<<msg<<", data bytes: "<<size<<std::endl;
    msg.insert(0, reinterpret_cast<char*>(&size), DATA_SIZE);
    return msg;
}

void Write(std::shared_ptr<tcp::socket> sock)
{
    int cnt = 0;
    // 发送100条消息给客户端
    while(cnt++ < MAX_WRITE_COUNT){
        std::string msg = GenerateMessage();
        sends.push_back(msg.size());
        boost::system::error_code ec;
        boost::asio::write(*sock, boost::asio::buffer(msg,msg.size()), ec);
        if(ec)
        {
            std::cout<<"write error, msg: "<<ec.message()<<std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //睡0.1秒
    }
}

void Read(std::shared_ptr<tcp::socket> sock)
{
    int cnt = 0;
    while(cnt++ < MAX_WRITE_COUNT)
    {
        char data[MESSAGE_LENGTH], *pos = data;
        memset(data, 0, MESSAGE_LENGTH);
        boost::system::error_code ec;
        size_t receive_size = sock->receive(boost::asio::buffer(data, MESSAGE_LENGTH));
        reads.push_back(receive_size);
        // Todo切包处理
        if(receive_size != 0){
            uint32_t data_size;
            strncpy(reinterpret_cast<char*>(&data_size), data, DATA_SIZE);
            pos = pos + DATA_SIZE; // 移动的大小为DATA_SIZE
            std::cout<<"client receive data bytes: "<<data_size<<", data is:"<<pos<<std::endl;
        }else
        {
            std::cout<<"server may be closed"<<std::endl;
            break;
        }
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: client.o <port>"<< std::endl;
        return 1;
    }
    try{
        srand(time(0));

        // 创建上下文服务
        boost::asio::io_context ioc;
        std::shared_ptr<tcp::socket> sock_ptr = std::make_shared<tcp::socket>(ioc);
        // 构造endpoint
        tcp::endpoint ep(tcp::v4(), atoi(argv[1]));
        boost::system::error_code ec = boost::asio::error::host_not_found;
        sock_ptr->connect(ep, ec);
        if(ec){
            std::cout << "connect failed, code is " << ec.value() << " error msg is " << ec.message() << std::endl;
			return 0;
        }
        // std::thread的函数不能接收引用参数 因为thread的参数是右值传递的 而接收参数为引用将报错
        // error: static assertion failed: std::thread arguments must be invocable after conversion to rvalue
        std::thread read_thread(Read, sock_ptr);
        std::thread write_thread(Write, sock_ptr);
        read_thread.join();
        write_thread.join();
        for(int i = 0; i < MAX_WRITE_COUNT; i++){
            std::cout<<"i = "<<i<<", write size: "<<sends[i]<<", read size: "<<reads[i]<<std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception: "<<e.what()<<std::endl;
    }
    return 0;
}
