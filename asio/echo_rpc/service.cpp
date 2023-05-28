#include"provider.h"
#include"echo.pb.h"

// 实现EchoSerice的服务方法
class EchoServiceImpl: public Echo::EchoService
{
public:
    void Echo(::google::protobuf::RpcController* controller,
                       const ::Echo::Request* request,
                       ::Echo::Response* response,
                       ::google::protobuf::Closure* done)
    {
        std::cout<<"Echo is called "<<std::endl;
        std::string msg = request->message();
        std::string send = "Send from server: " + msg;
        response->set_message(send);
        if(done)
        {
            done->Run();
        }
    }
};


int main()
{
    tcp::endpoint endpoint(address::from_string("127.0.0.1"), 12345);
    Provider provider(endpoint);
    google::protobuf::Service* service = new EchoServiceImpl();
    provider.RegisterService(service);
    provider.Run();
    return 0;
}