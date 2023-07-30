#include<future>
#include<iostream>
#include<thread>
#include<string>
#include<functional>
using namespace std;

int f(int x, int y)
{
    return x + y;
}

void task_lambda()
{
    packaged_task<int(int, int)> task([](int a, int b){ return a + b;});
    future<int> result = task.get_future();
    task(2, 9);
    cout<<"task lambda:\t"<<result.get()<<'\n';
}

void task_bind()
{
    packaged_task<int()> task(std::bind(f, 2, 11));
    std::future<int> result = task.get_future();
    task();
    cout<<"task bind:\t"<<result.get()<<'\n';
}

void task_thread()
{
    packaged_task<int(int, int)> task(f);
    std::future<int> result = task.get_future();
    std::thread task_td(std::move(task), 2, 10);
    task_td.join();
    cout<<"task thread:\t"<<result.get()<<'\n';
}

int main()
{
    task_lambda();
    task_bind();
    task_thread();
}