#include<boost/asio.hpp>
#include<iostream>
#include<vector>
#include<memory>
#include<unordered_set>
#include<queue>
#include"message.h"

using namespace boost::asio::ip;
typedef std::shared_ptr<tcp::socket> SockPtr;

class Participant{
public:
    virtual ~Participant(){};
    virtual void Deliver(Message msg) = 0;
};
typedef std::shared_ptr<Participant> ParticipantPtr;

// 聊天室 每个聊天室含有多个chatsession
class ChatRoom{
public:
    // 将session加入聊天室
    void Join(ParticipantPtr ptr){
        _participants.insert(ptr);
        // 将最近的msg发送给session
        for(auto msg: _recent_msgs){
            ptr->Deliver(msg);
        }
    }

    // 将session移除聊天室
    void Leave(ParticipantPtr ptr){
        _participants.erase(ptr);
    }

    void Deliver(Message msg, ParticipantPtr send_ptr){
        _recent_msgs.push_back(msg);
        while(_recent_msgs.size() > QUEUE_SIZE){
            _recent_msgs.pop_front();
        }

        // 将该消息发送给聊天室的其他所有成员
        for(const ParticipantPtr& ptr: _participants){
            if(ptr.get() != send_ptr.get()){
                ptr->Deliver(msg);
            }
        }
    }

private:
    enum{QUEUE_SIZE = 100};
    std::deque<Message> _recent_msgs;
    std::unordered_set<ParticipantPtr> _participants; // 当前聊天室的所有session指针
};


// 为了防止SessionPtr提前析构导致异步回调函数中this指针和room集合中的指针失效 需要继承enable_shared_from_this
class ChatSession: public Participant, public std::enable_shared_from_this<ChatSession>{
public:
    ChatSession(SockPtr sock, ChatRoom& room):_sock_ptr(sock), _room(room)
    {
    }

    void Start(){
        // 调用share_from_this
        _room.Join(shared_from_this());
        Read();
    }

    // 将msg加入写队列待发送给客户端_socket
    void Deliver(Message msg){
        bool writing = _write_msg.empty();
        _write_msg.push(msg);
        // 队列为空才调用write保证发送的顺序
        if(writing){
            Write();
        }
    }

private:
    
    void Write(){
        auto self(shared_from_this()); // 调用share_from_this传递给回调函数防止异步调用this失效
        boost::asio::async_write(*_sock_ptr, boost::asio::buffer(_write_msg.front().Data(), _write_msg.front().Space()), 
        [this, self](const boost::system::error_code& ec, size_t bytes){
            this->WriteHandler(ec, bytes);
        });
    }

    void WriteHandler(const boost::system::error_code& ec, size_t){
        if(!ec){
            _write_msg.pop();
            // 继续调用Write函数
            if(!_write_msg.empty()){
                Write();
            }
        }else{
            _room.Leave(shared_from_this());
            std::cout<<"async write error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
        }
    }

    void Read(){
        auto self(shared_from_this());
       _sock_ptr->async_read_some(boost::asio::buffer(_read_msg.Data(), _read_msg.Space()), 
        [this, self](const boost::system::error_code& ec, size_t bytes){
            this->ReadHandler(ec, bytes);
        }
        );
    }

    void ReadHandler(const boost::system::error_code& ec, size_t bytes)
    {
        if(!ec){
            // 将客户端发送的消息转发到聊天室其他客户端
            _read_msg.write_n(bytes);
            _room.Deliver(_read_msg, shared_from_this());
            _read_msg.Reset();
            Read();
        }else if(ec == boost::asio::error::eof)
        {
            std::cout<< "client "<< _sock_ptr->remote_endpoint().address().to_string() <<" has closed "<<" msg is "<<ec.message() << std::endl;
            _room.Leave(shared_from_this()); // 从聊天室中退出
        }else{
            std::cout<<"asyn read error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
            _room.Leave(shared_from_this()); // 从聊天室中退出
        }
    }

    SockPtr _sock_ptr;
    ChatRoom& _room;
    Message _read_msg;
    std::queue<Message> _write_msg; // 发送给客户端的消息队列
};


class Server{
public:
    Server(boost::asio::io_context& ioc, tcp::endpoint& ep):_ioc(ioc), _acceptor(ioc, ep){
        Accept();
    }

    void Accept(){
        // tcp::socket禁止拷贝 因此传递sock的指针给回调函数, 在回调函数中构造一个session
        // 为了防止局部对象被释放所以采用智能指针
        SockPtr sock = std::make_shared<tcp::socket>(_ioc);
        _acceptor.async_accept(*sock, std::bind(&Server::AcceptHandler, this, sock, std::placeholders::_1));
    }

private:

    void AcceptHandler(SockPtr sock_ptr, const boost::system::error_code& ec){
        if(!ec){
           std::make_shared<ChatSession>(sock_ptr, _room)->Start();
           // 继续接受新连接
           Accept();
        }else{
            std::cout<<"accept error value is "<<ec.value() <<" msg is "<<ec.message() << std::endl;
        }
    }

private:
    boost::asio::io_context& _ioc;
    tcp::acceptor _acceptor;
    ChatRoom _room;
};

int main(int argc, char* argv[]){
    try{
        if(argc != 2){
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }
        boost::asio::io_context ioc;
        tcp::endpoint endpoint(tcp::v4(), atoi(argv[1]));
        Server server(ioc, endpoint);
        ioc.run();
    }catch(std::exception& e){
        std::cerr << "Exception: "<<e.what() << std::endl;
    }
    return 0;
}