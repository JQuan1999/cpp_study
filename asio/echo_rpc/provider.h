#include<google/protobuf/service.h>
#include<google/protobuf/descriptor.h>
#include<unordered_map>
#include<string>
#include<memory>
#include<boost/asio.hpp>

#include"header.pb.h"

using namespace boost::asio::ip;
typedef std::shared_ptr<tcp::socket> SockPtr;

// 专门发布rpc服务的网络对象类
class Provider{
public:
    // ServiceInfo保存了服务和方法和的映射
    struct ServiceInfo{
        ServiceInfo():service(nullptr){};
        google::protobuf::Service* service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> method_map;
    };
    typedef std::shared_ptr<ServiceInfo> service_info_ptr;

    // 先调用_ioc的构造函数 再用_ioc传入_acceptor的构造函数 不然会传出段错误 为啥???
    Provider(tcp::endpoint& ep): _ioc(), _work(_ioc), _acceptor(_ioc, ep)
    {
    }
    void Run();
    void Accept();
    void AcceptHandler(SockPtr ptr, const boost::system::error_code& ec);
    void ReadSome(SockPtr ptr);
    void ReadHandler(SockPtr ptr, const boost::system::error_code& ec, size_t bytes);
    void WriteSome(SockPtr ptr, google::protobuf::Message*);
    void WriteHandler(SockPtr ptr, const boost::system::error_code& ec, size_t bytes);
    // 注册服务
    void RegisterService(google::protobuf::Service* service);

private:
    char write_buf[1024];
    char read_buf[1024];
    boost::asio::io_context::work _work;
    boost::asio::io_context _ioc;
    tcp::acceptor _acceptor;
    std::unordered_map<std::string, service_info_ptr> m_service; // 将服务名映射为ServiceInfo
};


void Provider::Run(){
    Accept();
    _ioc.run();
    std::cout<<"run over"<<std::endl;
}

void Provider::Accept()
{
    SockPtr sock_ptr = std::make_shared<tcp::socket>(_ioc);
    _acceptor.async_accept(*sock_ptr, std::bind(&Provider::AcceptHandler, this, sock_ptr, std::placeholders::_1));
}

// accept函数格式 void handler(const boost::system::error_code&)
void Provider::AcceptHandler(SockPtr ptr, const boost::system::error_code& ec){
    if(!ec){
        std::cout<<"accept new connection from "<<ptr->remote_endpoint().address().to_string()<<std::endl;
        ReadSome(ptr);
        Accept();
    }else{
        std::cout<<"accept error msg is "<<ec.message()<<std::endl;
        _acceptor.close();
    }
}


void Provider::ReadSome(SockPtr ptr)
{
    memset(read_buf, 0, 1024);
    ptr->async_read_some(boost::asio::buffer(read_buf, 1024), std::bind(&Provider::ReadHandler, this, ptr, std::placeholders::_1, std::placeholders::_2));
}


void Provider::ReadHandler(SockPtr ptr, const boost::system::error_code& ec, size_t bytes)
{
    if(!ec){
        uint32_t header_size = 0;
        strncpy((char*)&header_size, read_buf, 4); // 前4个字节为头部大小
        Header::RpcHeader head;
        std::string service_name;
        std::string method_name;
        uint32_t args_size;

        if(head.ParseFromArray(read_buf+4, header_size)){
            // 反序列化成功 获取head参数
            service_name = head.service_name(); 
            method_name = head.method_name();
            args_size = head.args_size();
        }else{
            // 反序列化失败
            std::cout<<"rpc header is ";
            std::cout.write(read_buf+4, header_size);
            std::cout<<"parse error!\n";
            return;
        }

        // 获取参数str
        std::string args_str(read_buf+4+header_size, read_buf+bytes);

        // 打印调试信息
        std::cout<<"=========================================="<<std::endl;
        std::cout << "header_size: " << header_size << std::endl; 
        std::cout<<"rpc header: ";
        std::cout.write(read_buf+4, header_size);
        std::cout<<std::endl;

        std::cout << "service_name: " << service_name << std::endl; 
        std::cout << "method_name: " << method_name << std::endl; 
        std::cout << "args_str: " << args_str << std::endl; 
        std::cout << "============================================" << std::endl;

        // 获取service对象
        auto iter = m_service.find(service_name);
        if(iter == m_service.end()){
            std::cout<<service_name<<" is not exist!"<<std::endl;
            return;
        }
        google::protobuf::Service* service = iter->second->service;

        // 找到method_name对应的method对象
        auto method_iter = iter->second->method_map.find(method_name);
        if(method_iter == iter->second->method_map.end()){
            std::cout<<service_name<<": "<<method_name<<" is not existed!"<<std::endl;
            return;
        }
        
        const google::protobuf::MethodDescriptor* method = method_iter->second; // 获取method对象
        
        // 生成rpc方法调用请求的request参数
        google::protobuf::Message* request = service->GetRequestPrototype(method).New();
        if(!request->ParseFromString(args_str)){
            std::cout<<"request parse error, request str: "<<args_str<<std::endl;
            return;
        }
        google::protobuf::Message* response = service->GetResponsePrototype(method).New();


        /***
        template <typename Class, typename Arg1, typename Arg2>
        inline Closure* NewCallback(Class* object, void (Class::*method)(Arg1, Arg2), Arg1 arg1, Arg2 arg2) {
            return new internal::MethodClosure2<Class, Arg1, Arg2> (object, method, true, arg1, arg2);
        }
        ***/
        // 回调函数
        google::protobuf::Closure* done = google::protobuf::NewCallback<Provider, SockPtr, google::protobuf::Message*>(this, &Provider::WriteSome, ptr, response);

        // 调用service提供的方法
        service->CallMethod(method, nullptr, request, response, done);

    }else if(ec == boost::asio::error::eof){
        std::cout<<"remote client has closed connection "<<std::endl;
        ptr->close();
        return;
    }
    else{
        std::cout<<"read error msg is "<<ec.message()<<std::endl;
        _acceptor.close();
    }
}


void Provider::WriteSome(SockPtr ptr, google::protobuf::Message* response)
{
    // 对response进行反序列化
    memset(write_buf, 0, 1024);
    if(response->SerializePartialToArray(write_buf, 1024))
    {
        boost::asio::async_write(*ptr, boost::asio::buffer(write_buf, strlen(write_buf)), 
        std::bind(&Provider::WriteHandler, this, ptr, std::placeholders::_1, std::placeholders::_2));
    }else{
        std::cout<<"serialize response error "<<std::endl;
        _acceptor.close();
    }
}


void Provider::WriteHandler(SockPtr ptr, const boost::system::error_code& ec, size_t bytes){
    if(!ec){
        std::cout<<"write "<<bytes<<" data to remote successed !"<<std::endl;
        ptr->close(); // 关闭sock模拟短链接
    }else{
        std::cout<<"write error msg is "<<ec.message()<<std::endl;
        _acceptor.close();
    }
}


void Provider::RegisterService(google::protobuf::Service* service)
{
    service_info_ptr s_ptr = std::make_shared<ServiceInfo>();
    s_ptr->service = service;

    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor* pservice = service->GetDescriptor();

    // 获取服务的名字
    std::string service_name = pservice->name();

    // 获取服务的方法数量
    int method_cnt = pservice->method_count();

    std::cout<<"service_name: "<<service_name<<std::endl;

    for(int i = 0; i < method_cnt; i++){
        const google::protobuf::MethodDescriptor* pmethod = pservice->method(i);
        std::string method_name = pmethod->name();
        s_ptr->method_map.insert({method_name, pmethod});
        std::cout<<"method_name: "<<method_name<<std::endl;
    }

    // 将服务名和s_ptr插入m_servie;
    m_service.insert({service_name, s_ptr});

}