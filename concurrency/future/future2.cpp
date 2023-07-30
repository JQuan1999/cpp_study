#include<future>
#include<iostream>
#include<thread>
#include<string>
using namespace std;

struct X
{
    int foo(int a, const std::string & s)
    {
        cout<<"thread id= "<<this_thread::get_id()<<" in foo int param = "<<a<<" string param = "<<s<<'\n'<<flush;
        return 1;
    }
    string bar(const std::string & s)
    {
        cout<<"thread id= "<<this_thread::get_id()<<" in bar string param = "<<s<<'\n'<<flush;
        string ret = s + "bar";
        return ret;
    }
    string async(const std::string& s)
    {
        cout<<"thread id= "<<this_thread::get_id()<<"in async string param = "<<s<<'\n'<<flush;
        string ret = s + "async";
        return ret;
    }
};
int main()
{
    X x;
    future<int> f1 = async(&X::foo, &x, 10, "hello");
    future<string> f2 = async(launch::deferred, &X::bar, &x, "world");
    future<string> f3 = async(launch::async, &X::async, &x, "test async");
    cout<<"thread id= "<<this_thread::get_id()<<'\n'<<flush;
    cout<<"f1.get(): "<<f1.get()<<'\n'<<flush;
    cout<<"f2.get(): "<<f2.get()<<'\n'<<flush;
    cout<<"f3.get(): "<<f3.get()<<'\n'<<flush;
}