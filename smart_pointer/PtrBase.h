#ifndef PTRBASE
#define PTRBASE
// shared_ptr和weak_ptr都有一个堆存储的计数器进行内存回收
// shared_ptr和weak_ptr的计数器是共享的，可能被多线程修改，因此需要mutex保护
#include<mutex>

class PtrBase{
public:
    // 计数器
    struct Counter{
        int uses = 0;
        int weaks = 0;
    };

    using p_counter = Counter*;
    using p_mutex = std::mutex*;
    // 堆计数器在多个ptr间共享
    p_counter counter;
    // 堆内存mutex保证线程安全，计数器为null才回收
    p_mutex mutex;

protected:
    // 这里用到了委托构造的技巧. 需要new计数器和互斥量
    // 注意由于用到new所以可能产生异常. 让异常逃离构造函数很麻烦, 因此用nothrow然后自己处理
    PtrBase() noexcept: PtrBase(new (std::nothrow) Counter(), new (std::nothrow) std::mutex) {}

    PtrBase(std::nullptr_t) noexcept: PtrBase() {}

    PtrBase(p_counter counter, p_mutex mutex) noexcept: counter(counter), mutex(mutex) {}

    void increase_shared_count() noexcept {
        if(counter != nullptr)
        {
            mutex->lock();
            ++(counter->uses);
            mutex->unlock();
        }
    }

    void increase_weak_count() noexcept {
        if(counter != nullptr)
        {
            mutex->lock();
            ++(counter->weaks);
            mutex->unlock();
        }
    }

    // 只要shared计数为0就返回给智能指针本身以回收目标对象的内存
    bool reduce_shared_count() noexcept{
        bool is_zero = true;
        if(counter != nullptr)
        {
            mutex->lock();
            --(counter->uses);
            if(counter->uses > 0)
            {
                is_zero = false;
            }
            mutex->unlock();
        }
        return is_zero;
    }

    // 只有当两种引用都为0才可以回收计数器本身的内存，记得对所有的堆内存都加锁
    void reduce_weak_count() noexcept{
        if(counter != nullptr)
        {
            mutex->lock();
            if(counter->weaks > 0)
            {
                --(counter->weaks);
            }
            if(counter->uses == 0 && counter->weaks == 0)
            {
                delete counter;
                counter = nullptr;
            }
            mutex->unlock();
        }
    }

    void check_mutex() noexcept
    {
        if(counter == nullptr)
        {
            delete mutex;
            mutex = nullptr;
        }
    }

    // new失败的时候做的补救措施
    void revert() noexcept {
        if (mutex != nullptr) {
            reduce_shared_count();
            reduce_weak_count();
            delete mutex;
            mutex = nullptr;
        }
        if (counter != nullptr) {
            delete counter;
            counter = nullptr;
        }
    }

    void swap(PtrBase& other) noexcept
    {
        std::swap(counter, other.counter);
        std::swap(mutex, other.mutex);
    }
};
#endif