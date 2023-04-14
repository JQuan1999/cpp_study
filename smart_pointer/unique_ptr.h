#include<algorithm>
#include<assert.h>


// uniqueptr
// public方法： 默认构造函数、显示构造函数(用指针T*)、移动构造、移动赋值、禁止拷贝构造和拷贝赋值、模仿原始指针的*运算符、->运算符
// 析构函数、赋值运算符

template<class T>
class UniquePtr{
    using pointer = T*;

public:
    typedef T ElementType;

    // 默认构造函数
    UniquePtr() noexcept: data(nullptr){}

    // 显示构造函数
    explicit UniquePtr(const pointer& data) noexcept: data(data){}

    // 析构函数
    ~UniquePtr() noexcept {_destory(*this);}

    // 移动构造和移动赋值都存在，用swap实现，移动后清空对方
    UniquePtr(UniquePtr&& moving) noexcept: data(nullptr){
        this->swap(moving);
        _destory(moving);
    }

    // 赋值运算符
    UniquePtr& operator=(UniquePtr&& moving) noexcept{
        if(this != &moving){
            swap(moving);
            _destory(moving);
        }
        return *this;
    }

    // 拷贝构造和拷贝赋值都被禁止
    UniquePtr(const UniquePtr&) noexcept = delete;
    UniquePtr& operator=(const UniquePtr&) noexcept = delete;

    // 仅允许实验nullptr进行拷贝赋值
    UniquePtr& operator=(std::nullptr_t) noexcept{
        reset();
        return *this;
    }

    // 显示bool转换函数
    explicit operator bool() const noexcept { return data != nullptr;}

    // 智能指针都需要模仿原始指针，不要对null操作
    T& operator*() const noexcept {
        assert(data != nullptr);
        return *data;
    }

    pointer operator-> () const noexcept{
        assert(data != nullptr);
        return data;
    }

    pointer get() const { return data; }


    void swap(UniquePtr& other) noexcept{
        std::swap(data, other.data);
    }

    void swap(UniquePtr&& other) noexcept{
        swap(data, other.data);
    }

    void reset(pointer p = nullptr) noexcept{
        swap(UniquePtr(nullptr));
    }

private:

    // 清空并置为null
    void _destory(UniquePtr& target) noexcept{
        if(target.data != nullptr){
            delete target.data;
            target.data = nullptr;
        }
    }

private:
    pointer data;
};