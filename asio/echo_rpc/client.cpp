#include<iostream>
#include<string>
#include"channel.h"
#include"echo.pb.h"


int main()
{
    Channel* channel = new Channel();
    channel->SetRemote("127.0.0.1", 12345);
    
    Echo::EchoService_Stub stub(channel);
    const google::protobuf::MethodDescriptor* method = Echo::EchoService::descriptor()->method(0);
    std::string method_name = method->name();
    std::string full_name = method->full_name();
    std::cout<<"method name = "<<method_name<<" full_name = "<<full_name<<std::endl;
    
    Echo::Request request;
    request.set_message("hello world");
    Echo::Response response;

    // 发起调用将结果返回在response中
    // channel_->CallMethod(descriptor()->method(0), controller, request, response, done);
    stub.Echo(nullptr, &request, &response, nullptr);
    std::cout<<"return response: "<<response.message()<<std::endl;

    delete channel;
    return 0;
}