#include<mutex>
#include<thread>
#include<atomic>
#include<condition_variable>
#include<unistd.h>
#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;

#define RUN_TIMES 100000
#define THREAD_NUM 10

class SpinLock{
public:
    SpinLock():_flag(false){}
    ~SpinLock(){}
    void Lock()
    {
        bool expect = false;
        // 当flag为true时会compare失败 将expect设为true并返回false
        while(!_flag.compare_exchange_strong(expect, true))
        {
            expect = false;
        }
    }
    void UnLock()
    {
        _flag.store(false);
    }
private:
    SpinLock(const SpinLock&);
    SpinLock& operator=(const SpinLock&);
    atomic<bool> _flag;
};

static atomic<int> at_total(0);
static long total = 0;
mutex m;
SpinLock spin;

void InThread()
{
    for(int i = 0; i < RUN_TIMES; i++)
    {
        total++;
    }
}

void InAtomic()
{
    for(int i = 0; i < RUN_TIMES; i++)
    {
        at_total++;
    }
}

void InMutex()
{
    for(int i = 0; i < RUN_TIMES; i++)
    {
        m.lock();
        total++;
        m.unlock();
    }
}

void InSpinLock()
{
    for(int i = 0; i < RUN_TIMES; i++)
    {
        spin.Lock();
        total++;
        spin.UnLock();
    }
}

int main()
{
    // test multi thread
    vector<thread> threads;
    clock_t start = clock();
    for(int i = 0; i < THREAD_NUM; i++){
        threads.emplace_back(InThread);
    }
    for_each(threads.begin(), threads.end(), [](std::thread& t){if(t.joinable()) t.join(); });
    clock_t end = clock();
    cout<<"For multi thread total = "<<total<<endl;
    cout<<"Run time = "<<end-start<<endl;

    // atomic
    threads.clear();
    start = clock();
    for(int i = 0; i < THREAD_NUM; i++){
        threads.emplace_back(InAtomic);
    }
    for_each(threads.begin(), threads.end(), [](std::thread& t){if(t.joinable()) t.join(); });
    end = clock();
    cout<<"For atomic total = "<<at_total.load()<<endl;
    cout<<"Run time = "<<end-start<<endl;

    // mutex
    total = 0;
    threads.clear();
    start = clock();
    for(int i = 0; i < THREAD_NUM; i++){
        threads.emplace_back(InMutex);
    }
    for_each(threads.begin(), threads.end(), [](std::thread& t){if(t.joinable()) t.join(); });
    end = clock();
    cout<<"For mutex total = "<<total<<endl;
    cout<<"Run time = "<<end-start<<endl;

    // spinlock
    total = 0;
    threads.clear();
    start = clock();
    for(int i = 0; i < THREAD_NUM; i++){
        threads.emplace_back(InSpinLock);
    }
    for_each(threads.begin(), threads.end(), [](std::thread& t){if(t.joinable()) t.join(); });
    end = clock();
    cout<<"For spinlock total = "<<total<<endl;
    cout<<"Run time = "<<end-start<<endl;

    return 0;
}