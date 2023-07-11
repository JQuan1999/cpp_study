#include<mutex>
#include<thread>
#include<atomic>
#include<condition_variable>
#include<unistd.h>
#include<vector>
#include<memory>
#include<algorithm>
#include<iostream>

using namespace std;

#define WRITE_TIMES 10000000
#define READ_TIMES 10000000
#define READ_THREADS_NUM 4
#define WRITE_THREADS_NUM 4

class RwLock;
typedef shared_ptr<RwLock> RwLockPtr;
class RwLock1;
typedef shared_ptr<RwLock1> RwLockPtr1;
class RwLock2;
typedef shared_ptr<RwLock2> RwLockPtr2;
class RwLock3;
typedef shared_ptr<RwLock3> RwLockPtr3;

static atomic<int> read_t;
static atomic<int> write_t;

class RwLock
{
private:
    RwLock(const RwLock&);
    RwLock& operator=(const RwLock&);

public:
    RwLock(){};
    virtual ~RwLock(){};
    virtual void Lock() = 0;
    virtual void UnLock() = 0;
    virtual void LockInShared() = 0;
    virtual void UnLockInShared() = 0;
};


// 读者优先
class RwLock1: public RwLock{
public:
    RwLock1():_read_count(0){}
    
    ~RwLock1(){}

    void Lock()
    {
        _m.lock();
    }

    void UnLock()
    {
        _m.unlock();
    }

    void LockInShared()
    {
        if(++_read_count == 1)
        {
            _m.lock();
        }
    }

    void UnLockInShared()
    {
        if(--_read_count == 0)
        {
            _m.unlock();
        }
    }
private:
    mutex _m;
    atomic<int> _read_count;
};

// 写者优先
class RwLock2: public RwLock
{
public:
    RwLock2()
        : _read_count(0)
        , _write_count(0)
    {}
    
    ~RwLock2(){}

    void Lock()
    {
        if(++_write_count == 1)
        {
            _m2.lock();
        }
        _m1.lock();
    }

    void UnLock()
    {
        if(--_write_count == 0)
        {
            _m2.unlock();
        }
        _m1.unlock();
    }

    void LockInShared()
    {
        _m2.lock(); // 在申请m1_lock前要申请m2_lock 当没有写线程时才释放m2_lock实现写优先
        if(++_read_count == 1)
        {
            _m1.lock();
        }
        _m2.unlock();
    }

    void UnLockInShared()
    {
        if(--_read_count == 0)
        {
            _m1.unlock();
        }
    }

private:
    mutex _m1;
    mutex _m2;
    atomic<int> _read_count;
    atomic<int> _write_count;
};


class RwLock3: public RwLock
{
public:
    RwLock3()
        : _read_count(0)
    {}
    
    ~RwLock3(){}

    // 读者优先中只要读者到达，读者就可以进入等待队列，而写者必须等待读者读完，直到没有新读者到达，才可以进行写
    // m2就是为了阻止读者一到达就可以进行读，例如一些读者正在队列读数据，当一个写者到达时占有m2，当正在读队列的读者读完之后释放m1
    // 写者占有了m1和m2，此后新来的读者必须等待写者释放m2后才可以进行读
    void Lock()
    {
        _m2.lock(); 
        _m1.lock();
    }

    void UnLock()
    {
        _m1.unlock();
        _m2.unlock();
    }

    void LockInShared()
    {
        _m2.lock();
        if(++_read_count == 1)
        {
            _m1.lock();
        }
        _m2.unlock();
        ++_read_count;
    }

    void UnLockInShared()
    {
        if(--_read_count == 0)
        {
            _m1.unlock();
        }
    }

private:
    mutex _m1;
    mutex _m2;
    atomic<int> _read_count;
};

void Write(RwLockPtr lock)
{
    clock_t start = clock();
    while(write_t.load() < WRITE_TIMES)
    {
        lock->Lock();
        ++write_t;
        lock->UnLock();
    }
    clock_t end = clock();
    cout<<"Thread id= "<<this_thread::get_id()<<" Write Finish total time= "<<end - start<<" write count= "<<write_t.load()<<endl;
}

void Read(RwLockPtr lock)
{
    clock_t start = clock();
    while(read_t.load() < READ_TIMES)
    {
        lock->Lock();
        ++read_t;
        lock->UnLock();
    }
    clock_t end = clock();
    cout<<"Thread id= "<<this_thread::get_id()<<" Read Finish total time= "<<end - start<<" Read count= "<<read_t.load()<<endl;
}

void Test(const RwLockPtr& ptr)
{
    vector<thread> w_threads, r_threads;
    write_t.store(0);
    read_t.store(0);
    for(int i = 0; i < WRITE_THREADS_NUM; i++)
    {
        w_threads.emplace_back(Write, ptr);
    }
    for(int i = 0; i < READ_THREADS_NUM; i++)
    {
        r_threads.emplace_back(Read, ptr);
    }
    for_each(r_threads.begin(), r_threads.end(), [](std::thread& t){t.join(); });
    for_each(w_threads.begin(), w_threads.end(), [](std::thread& t){t.join(); });
}

int main()
{
    RwLockPtr1 ptr1(new RwLock1());
    RwLockPtr2 ptr2(new RwLock2());
    RwLockPtr3 ptr3(new RwLock3());
    Test(ptr1);
    Test(ptr2);
    Test(ptr3);
    return 0;
}