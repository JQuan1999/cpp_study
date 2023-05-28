#include<boost/asio.hpp>
#include<thread>
#include<vector>
#include<iostream>

class ThreadGroup{
public:
    typedef std::function<void()> func;
    ThreadGroup(int size):_size(size), ioc(), work_ptr(new boost::asio::io_context::work(ioc)){
        for(int i = 0; i < size; i++){
            threads.emplace_back([this](){ ioc.run(); }); // 每个线程都在一个无限循环中运行，等待io_service调度的任务
        }
    }
    // ioc
    void post(func& task){
        ioc.post(task); // 将task分配给线程池线程
    }

    void dispatch(func& task){
        ioc.dispatch(task); // dispatch立即执行
    }

    void stop()
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
    std::vector<std::thread> threads;
    int _size;
    boost::asio::io_context ioc;
    boost::asio::io_context::work* work_ptr;
};

int main(){
    ThreadGroup thread_group(2);
    for(int i = 0; i < 100; i++){
        ThreadGroup::func task = [i](){
            std::cout<<"Thread id = "<<std::this_thread::get_id()<<" i = "<<i<<std::endl;
        };
        if(i < 90){
            thread_group.post(task);
        }else{
            thread_group.dispatch(task);
        }
    }
    thread_group.stop();
    return 0;
}