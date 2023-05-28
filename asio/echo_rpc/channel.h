#include <google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<google/protobuf/message.h>
#include<boost/asio.hpp>
#include<string>
#include<iostream>
#include"header.pb.h"

using namespace boost::asio::ip;

class Channel: public google::protobuf::RpcChannel{
public:
    Channel(): _socket(ioc)
    {
    }

    void SetRemote(const std::string ip, short port){
        _socket.async_connect(tcp::endpoint(address::from_string(ip), port), std::bind(&Channel::ConnechHandler, this, std::placeholders::_1));
        ioc.run();
    }

    // 所有通过stub代理对象调用的rpc方法都走到了这里 统一做rpc方法调用的数据序列化和网络发送
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller,
                        const google::protobuf::Message* message,
                        google::protobuf::Message* response,
                        google::protobuf::Closure* done);
private:
    void ConnechHandler(const boost::system::error_code& ec){
        if(ec){
           std::cout<<"connect error erorr msg is "<<ec.message()<<std::endl;
           _socket.close();
        }else{
            std::cout<<"connect sucessful remote address is "<<_socket.remote_endpoint().address().to_string()<<std::endl;
        }
    }
    boost::asio::io_context ioc;
    tcp::socket _socket;
};


// 所有的stub代理对象调用的方法都调用rpc框架的方法
// 1. 将request进行解析 2.组装成新的消息 send_data = size + rpc_header + param 
// 3.检查服务是否存在 找到rpc注册服务的ip与端口 4. 建立连接发送请求 5.收到响应消息
void Channel::CallMethod(const google::protobuf::MethodDescriptor* method,
                        google::protobuf::RpcController* controller,
                        const google::protobuf::Message* request,
                        google::protobuf::Message* response,
                        google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();
    
    // 获取参数的序列化字节长度
    uint32_t args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str)){
        args_size = args_str.size();
    }else{
        std::cout<<"serialize request error!"<<std::endl;
        return ;
    }

    // 定义rpc的请求头部
    // header是客户端和服务端通信的message
    Header::RpcHeader header;
    header.set_service_name(service_name);
    header.set_method_name(method_name);
    header.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(header.SerializeToString(&rpc_header_str)){
        header_size = rpc_header_str.size();
    }else{
        std::cout<<"serialize header error!"<<std::endl;
        return;
    }

    // 组织待发送的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4)); // header size
    send_rpc_str += rpc_header_str; // rpc header
    send_rpc_str += args_str; // args

    // 打印调试信息
    std::cout<<"==========================================="<<std::endl;
    std::cout<<"header_size: "<<header_size<<std::endl;
    std::cout<<"rpc_header_str: "<<rpc_header_str<<std::endl;
    std::cout<<"service_name: "<<service_name<<std::endl;
    std::cout<<"method_name: "<<method_name<<std::endl;
    std::cout<<"args_str: "<<args_str<<std::endl;
    std::cout<<"==========================================="<<std::endl;

    if(!_socket.is_open()){
        std::cout<<"socket has not connected to remote"<<std::endl;
        return;
    }
    boost::system::error_code ec;
    boost::asio::write(_socket, boost::asio::buffer(send_rpc_str.c_str(), send_rpc_str.size()), ec);
    if(ec){
        std::cout<<"write error erorr msg is "<<ec.message()<<std::endl;
        _socket.close();
        return;
    }

    char buf[1024];
    memset(buf, 0, 1024);
    size_t read_size = _socket.read_some(boost::asio::buffer(buf, 1024), ec);
    if(ec){
        std::cout<<"read error erorr msg is "<<ec.message()<<std::endl;
        _socket.close();
        return;
    }

    if(!response->ParseFromArray(buf, read_size)){
        std::cout<<"parse erorr! response str: ";
        std::cout.write(buf, read_size);
        std::cout<<std::endl;
        _socket.close();
        return ;
    }

    _socket.close();
}