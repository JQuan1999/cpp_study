#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
 
using boost::asio::ip::tcp;
 
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}
 
class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>    
// 神奇的enable_shared_from_this帮助我们不用担心异步调用时的内存失效
 
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;
 
  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new tcp_connection(io_service));
  }
 
  tcp::socket& socket()
  {
    return socket_;
  }
 
  void start()
  {
    message_ = make_daytime_string();
 
// 使用异步写函数将message_发送给客户端。
// 这个函数用了两个神奇的东西，boost::bind和shared_from_this。
// 熟悉stl::bind的朋友应该可以很容易理解boost::bind的作用。
// 主要作用就是注册异步操作的回调函数，这里函数指针是tcp_connection::handle_write
// 由于是类的非静态成员函数，所以需要this或者shared_ptr这种定义get_pointer函数的类。
//  shared_ptr源码中有这句注释：”get_pointer() enables boost::mem_fn to recognize shared_ptr”
// 对bind原理感兴趣的朋友见：http://kelvinh.github.io/blog/2013/12/03/boost-bind-illustrated/
//  shared_from_this是派生自boost::enable_shared_from_this的函数，可以保证tcp_connection在异步操作时不被析构
// 试想下如果异步接收/发送数据的时候，数据内存地址被释放了，后果很严重！
// 对原理感兴趣的朋友见：http://hahaya.github.io/use-enable-shared-from-this/
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
 
private:
  tcp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }
 
// 这就是那个回调函数，到达这里的情况有两种，一是数据全部发送完毕，二是发送了error。
  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }
 
  tcp::socket socket_;
  std::string message_;
};
 
class tcp_server
{
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 12345))
  {
    start_accept();
  }
 
private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_io_service());
 
// 异步接收连接，注意此处为什么没有像上面tcp_connection那样使用shared_from_this？
// 请往下看
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }
 
// 此处是接收连接之后的回调函数
  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }
 
// 注意这个函数，这就像一个无尽循环一样，所以不怕超出作用域，而被析构掉。也就不需要shared_from_this。
    start_accept();
  }
 
  tcp::acceptor acceptor_;
};
 
int main()
{
  try
  {
    boost::asio::io_service io_service;
    tcp_server server(io_service);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
 
  return 0;
}