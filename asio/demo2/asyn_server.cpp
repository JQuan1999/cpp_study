#include"server.h"

int main(){
    try{
        boost::asio::io_context ioc;
        Server server(ioc, 12345);
        ioc.run();
    }
    catch(std::exception& e){
        std::cout<<"catch error "<< e.what() << std::endl; 
    }
    return 0;
}