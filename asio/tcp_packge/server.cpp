#include<iostream>
#include<boost/asio.hpp>
#include<thread>
#include<string>
#include<deque>
#include<memory>
#include<set>
#include<functional>
#include<vector>
#include<atomic>
#include<cmath>
using namespace boost::asio::ip;

/*
    测试tcp粘包处理 客户端发送数据 服务端2s接收一次将粘包的数据进行拆分
    自定义消息格式：消息头部(4byte)+消息体
*/
const int HEADER_SIZE = 4; // 头部大小
const int DATA_LENGTH = 128; // 数据大小
const int MESSAGE_LENGTH = HEADER_SIZE + DATA_LENGTH; // 消息大小
const int RECIEVE_DATA_LENGTH = 2 * MESSAGE_LENGTH; // 接收的缓冲区大小

class Session: public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_context& ioc)
        : _socket(ioc)
        , _message_complete(false)
        , _write_free(0)
        , _write_lock(1)
        , _read_free(0)
        , _read_lock(1)
        , _read_token(_write_free)
        , _write_token(_read_free)
    {

    }

    void Start()
    {
        StartRead();
        StartWrite();
    }

    tcp::socket& GetSocket()
    {
        return _socket;
    }

    void SendMessage(std::string msg)
    {
        PutItem(msg);
        StartWrite();
    }

    void StartWrite()
    {
        if(_write_token.compare_exchange_strong(_write_free, _write_lock))
        {
            if(GetItem())
            {
                _socket.async_write_some(boost::asio::buffer(_send_message, _send_message.size()), 
                                        std::bind(&Session::OnWrite, shared_from_this(),
                                        std::placeholders::_1, std::placeholders::_2));
            }else{
                _write_token.compare_exchange_strong(_write_lock, _write_free);
                // std::cout<<"StartWrite(): send failed queue is empty"<<std::endl;
                return;
            }
        }else
        {
            _write_free = 0; // compare_exchange_strong返回false说明_write_token为1 需要恢复_write_free为0
        }
    }

    void PutItem(std::string msg)
    {
        {
            std::lock_guard<std::mutex> lock(_send_queue_mutex);
            _send_pending_queue.push_back(msg);
        }
    }

    void CleanRecieveEnv()
    {
        memset(_receieve_msg, 0, sizeof(_receieve_msg));
        _cur_pos = _receieve_msg;
        if(_message_complete) // 接受到的消息完整需要重置
        {
            CleanReceiveMessage();
        }
    }

    void CleanReceiveMessage()
    {
        _message = "";
        _recieve_size = 0;
        _message_complete = false;
    }

    void StartRead()
    {
        // 原子的检查_read_token为_free的时候设置为_lock并返回true表明没有线程正在读
        // 下一个读的线程将获取_free失败
        if(_read_token.compare_exchange_strong(_read_free, _read_lock))
        {
            CleanRecieveEnv();
            _socket.async_read_some(boost::asio::buffer(_receieve_msg, RECIEVE_DATA_LENGTH), 
                                std::bind(&Session::OnRead, shared_from_this(), 
                                std::placeholders::_1, std::placeholders::_2));
        }
        else
        {
            _read_free = 0; // 恢复_read_free为0
        }
    }

    void OnRead(const boost::system::error_code& ec, size_t bytes)
    {
        if(ec)
        {
            std::cout<<"read error, msg: "<<ec.message()<<std::endl;
            // Todo 关闭socket 删除session
            return;
        }
        std::cout<<"recieve msg: ";
        std::cout.write(_receieve_msg, bytes);
        std::cout<<" ,bytes = "<<bytes<<std::endl;
        // 开始解析收到的数据
        while(bytes > 0)
        {
            // 接收数据头部不完全
            if(_recieve_size < HEADER_SIZE)
            {
                // HEADER_SIZE - _receive_size计算出剩余的头部字节数, 因为全部剩余的bytes数据可能比这个数还小所以需要取min
                uint32_t res_header_size = HEADER_SIZE - _recieve_size;
                res_header_size = std::min(static_cast<size_t>(res_header_size), bytes);
                _message.append(_cur_pos, res_header_size);
                // 更新大小并移动解析位置的指针
                _recieve_size += res_header_size;
                _cur_pos += res_header_size;
                bytes -= res_header_size;
            }else
            {
                // 解析出消息的大小
                uint32_t data_size;
                // static_cast 不允许没有关系的两个类指针之间的转换
                // reinterpret_cast 编译时行为，不产生代码；static_cast 运行时行为，产生代码
                strncpy(reinterpret_cast<char*>(&data_size), &_message[0], 4);
                uint32_t res_data_size = data_size + HEADER_SIZE - _recieve_size; // 剩余数据部分 = 数据大小 + 头部大小 - 已接受的大小
                // Todo网络字节序转主机字节序
                res_data_size = std::min(static_cast<size_t>(res_data_size), bytes); // 剩余部分res_data_size和剩余bytes取min为得到需要解析的部分

                _message.append(_cur_pos, res_data_size);
                std::cout<<"res_data_size = "<<res_data_size<<", _message = "<<_message<<std::endl;
                _recieve_size += res_data_size;
                _cur_pos += res_data_size;
                bytes -= res_data_size;
                // 如果_receive_size == data_size + HEADER_SIZE表明解析到了一条完整的消息
                if(_recieve_size == data_size + HEADER_SIZE)
                {
                    std::cout<<"parse a complete msg, msg len: "<<data_size<<" , msg:"<<_message.substr(4)<<std::endl;
                    _message_complete = true; // 设置为true 表明已收到完整的一条消息
                    SendMessage(_message); // 将消息重新发送给客户端
                    if(bytes > 0)
                    {
                        // 如果还有数据需要解析 重置当前消息临时变量
                        CleanReceiveMessage();
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(2)); // 睡眠2秒以模拟粘包
        // 解析完毕释放lock 继续读数据
        _read_token.compare_exchange_strong(_read_lock, _read_free);
        StartRead();
    }

    bool GetItem()
    {
        std::lock_guard<std::mutex> lock(_send_queue_mutex);
        if(!_send_pending_queue.empty())
        {
            _send_message = _send_pending_queue.front();
            _send_pending_queue.pop_front();
            return true;
        }else
        {
            return false;
        }
    }

    void OnWrite(const boost::system::error_code& ec, size_t bytes)
    {
        if(ec)
        {
            std::cout<<"write error, msg: "<<ec.message()<<std::endl;
            // Todo close socket
            return;
        }else
        {
            if(bytes < _send_message.size())
            {
                _send_message.erase(0, bytes);
                _socket.async_write_some(boost::asio::buffer(_send_message, _send_message.size()), 
                                        std::bind(&Session::OnWrite, shared_from_this(),
                                        std::placeholders::_1, std::placeholders::_2));
            }else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 睡眠0.1秒防止客户端粘包
                _write_token.compare_exchange_strong(_write_lock, _write_free);
                StartWrite();
            }
        }
    }

private:
    char _receieve_msg[RECIEVE_DATA_LENGTH];

    uint32_t _recieve_size; // 接收消息的临时变量 记录收到的消息大小
    bool _message_complete; // 接收消息的临时变量 接收消息是否完整
    std::string _message; // 接收消息的临时变量 解析到的一条消息
    char* _cur_pos; // 接收消息的临时变量 解析到的位置初始位置为_receieve_msg首地址

    std::string _send_message; // 发送的临时变量

    int _write_free;
    int _write_lock;
    std::atomic<int> _write_token;
    int _read_free;
    int _read_lock;
    std::atomic<int> _read_token;
    std::mutex _send_queue_mutex;
    std::deque<std::string> _send_pending_queue; // Todo流量控制 控制队列的发送大小
    tcp::socket _socket;
};

class Server: public std::enable_shared_from_this<Server>
{
public:
    typedef std::shared_ptr<Session> SessionPtr;
    Server(tcp::endpoint& endpoint)
        : _endpoint(endpoint)
        , _acceptor(_ioc, _endpoint)
    {

    }

    boost::asio::io_context& GetIoService()
    {
        return _ioc;
    }

    void Start()
    {
        Accept();
    }

    void Accept()
    {
        SessionPtr session_ptr = std::make_shared<Session>(_ioc);
        _acceptor.async_accept(session_ptr->GetSocket(), std::bind(&Server::OnAccept, shared_from_this(), session_ptr, std::placeholders::_1));
    }

private:
    void OnAccept(SessionPtr session_ptr, const boost::system::error_code& ec)
    {
        if(ec)
        {
            std::cout<<"accept error, msg: "<<ec.message()<<std::endl;
            return;
        }else{
            session_ptr->Start();
            _sessions.insert(session_ptr);
            Accept();
        }
    }
private:
    std::set<SessionPtr> _sessions;
    boost::asio::io_context _ioc;
    tcp::endpoint _endpoint;
    tcp::acceptor _acceptor;
};


class ThreadGroup{
public:
    typedef std::function<void()> func;
    ThreadGroup(int size, boost::asio::io_context& ioc)
        :_size(size)
        , _ioc(ioc)
        , work_ptr(new boost::asio::io_context::work(_ioc))
    {
        for(int i = 0; i < size; i++)
        {
            threads.emplace_back([this](){ _ioc.run(); }); // 每个线程都在一个无限循环中运行，等待io_service调度的任务
        }
    }
    ~ThreadGroup()
    {
        Stop();
    }

    void Post(func& task){
        _ioc.post(task); // 将task分配给线程池线程
    }

    void Dispatch(func& task)
    {
        _ioc.dispatch(task); // dispatch立即执行
    }

    void Stop()
    {
        // ioc.stop()会在完成分配的任务后退出
        // 通过删除io_service::work对象，让io_service的work失效，从而让io_service在所有任务完成后停止运行
        // 在让io_service的work失效之前，需要确保所有需要提交的任务已经被提交到io_service中
        delete work_ptr; 
        work_ptr = nullptr;
        for(int i = 0; i < _size; i++){
            threads[i].join();
        }
    }
private:
    ThreadGroup(const ThreadGroup&) =delete;
    void operator=(const ThreadGroup&)=delete;

private:
    std::vector<std::thread> threads;
    int _size;
    boost::asio::io_context& _ioc;
    boost::asio::io_context::work* work_ptr;
};


int main()
{
    tcp::endpoint ep(tcp::v4(), 12345);
    // Server server(ep); // Error 在栈上定义继承了enable_shared_from_this的类使用enable_share_from_this会报错
    std::shared_ptr<Server> server = std::make_shared<Server>(ep);
    ThreadGroup thread_group(2, server->GetIoService());
    server->Start();
    return 0;
}