// 共享指针实现
// 构造函数: 默认构造、原始指针构造、拷贝构造、移动构造
// 赋值函数：赋值函数、移动赋值
// operator*, operator->
// 析构函数
#ifndef MYSHAREPTR
#define MYSHAREPTR
#include<assert.h>
#include<algorithm>

template<typename T>
class SharePtr{
    using pointer = T*;

private:
    pointer data;
    int* count;

public:
    // 默认构造函数
    SharePtr(): data(nullptr), count(new int(0)) {}

    // 裸指针直接构造
    SharePtr(T* ptr): data(ptr), count(new int(0)) {
        if(data != nullptr)
        {
            ++(*count);
        }
    }

    // 拷贝构造
    SharePtr(const SharePtr& ptr): data(ptr.data), count(ptr.count) {
        if(data != nullptr)
        {
            ++(*count);
        }
    }

    // 拷贝赋值, 采用copy-swap写法
    SharePtr& operator=(const SharePtr& ptr){
        SharePtr tmp(ptr);
        swap(tmp);
        return *this;
    }

    // 移动构造函数
    SharePtr(SharePtr&& ptr): data(nullptr), count(nullptr){
        swap(ptr);
        _destory(ptr);
    }

    // 移动赋值
    SharePtr& operator=(SharePtr&& ptr){
        if(this != &ptr)
        {
            swap(ptr);
            _destory(ptr);
        }
        return *this;
    }

    // 老三样
    pointer operator->() const noexcept { assert(data != nullptr); return data; }
    T& operator*() const noexcept { assert(data != nullptr); return *data; }
    pointer get() const noexcept { return data; }

    pointer get() {assert(data != nullptr); return data;};
    int use_count() {assert(count != nullptr); return *count;}

    void swap(SharePtr& ptr)
    {
        std::swap(data, ptr.data);
        std::swap(count, ptr.count);
    }

    void swap(SharePtr&& ptr)
    {
        std::swap(data, ptr.data);
        std::swap(count, ptr.count);
    }

    ~SharePtr(){
        _destory(*this);
    }

private:

    void _destory(SharePtr& ptr)
    {
        if(ptr.data != nullptr)
        {
            if(--(*ptr.count) == 0)
            {
                delete ptr.data;
                delete ptr.count;
                ptr.data = nullptr;
                ptr.count = nullptr;
            }
        }        
    }
};
#endif