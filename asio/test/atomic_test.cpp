#include<iostream>
#include<atomic>
using namespace std;

int _free = 0;
int _lock = 1;

int main()
{
    std::atomic<int> _send_lock(0);
    _send_lock.compare_exchange_strong(_free, _lock);
    std::cout<<"_send_lock value = "<<_send_lock.load()<<", _free = "<<_free<<" _lock = "<<_lock<<std::endl;
    _send_lock.compare_exchange_strong(_free, _lock);
    std::cout<<"_send_lock value = "<<_send_lock.load()<<", _free = "<<_free<<" _lock = "<<_lock<<std::endl;
    return 0;
}