#include<future>
#include<iostream>
#include<thread>
using namespace std;

int main()
{
    packaged_task<int()> task([](){return 7;});
    future<int> f1 = task.get_future();
    thread t(move(task)); // launch on a thread

    // future from an async()
    future<int> f2 = async(launch::async, [](){return 8;});

    // future from a promise
    promise<int> p;
    future<int> f3 = p.get_future();
    thread([&p](){p.set_value_at_thread_exit(9);}).detach();

    cout<<"Waiting...."<<endl;
    f1.wait();
    f2.wait();
    f3.wait();
    cout<<"Done\nResults are: "<<f1.get()<<' '<<f2.get()<<' '<<f3.get()<<'\n';
    t.join();
    
}
