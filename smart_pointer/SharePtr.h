#ifndef SHAREPTR
#define SHAREPTR

#include"PtrBase.h"
#include"WeakPtr.h"

template<typename T>
class SharedPtr : public PtrBase {
    using pointer = T*;
    // 需要和WeakPtr形成friend方便两者之间的转型
    friend class WeakPtr<T>;

private:
    // 原生指针
    pointer data;

private:
    // 先减少计数, 如果为0则释放资源
    void _destroy(SharedPtr& target) noexcept {
        if (data != nullptr) {
            if (target.reduce_shared_count()) {
                delete target.data;
                target.data = nullptr;
            }
            target.check_mutex();
        }
    }

    // 给WeakPtr用的构造
    SharedPtr(const WeakPtr<T>& wptr) noexcept : data(wptr.data), PtrBase(wptr.counter, wptr.mutex) {
        increase_shared_count();
    }

public:
    // 用于外部获取指针原始类型
    typedef T ElementType;
    
    // 默认构造函数, 全部赋为nullptr
    SharedPtr() noexcept : data(nullptr), PtrBase() {};

    // 记得显式以防止隐式类型转换
    explicit SharedPtr(const pointer& data) noexcept :
        data(data), PtrBase() {
        // nullptr代表空间申请失败
        if (counter == nullptr || mutex == nullptr) {
            this->data = nullptr;
            revert();
        }
        if (data != nullptr) {
            increase_shared_count();
        }
    }

    ~SharedPtr() noexcept {
        _destroy(*this);
    }

    // 拷贝构造, shared_ptr拷贝后会将计数器+1
    SharedPtr(const SharedPtr& copy) noexcept : data(copy.data), PtrBase(copy.counter, copy.mutex) {
        if (data != nullptr) {
            increase_shared_count();
        }
    }
    // 拷贝赋值, 采用copy-swap写法, 由于右值引用的存在, 折叠式写法会造成二义性
    // 旧的内存会由于tmp的析构而释放, 新的内存的申请也在tmp的拷贝构造中完成了
    SharedPtr& operator=(const SharedPtr& copy) noexcept {
        SharedPtr tmp(copy);
        swap(tmp);
        return *this;
    }
    // 用nullptr赋值时相当于清空
    SharedPtr& operator=(std::nullptr_t) noexcept {
        _destroy(*this);
        return *this;
    }

    // 移动构造函数, 由于是构造所以可以直接夺取指针内容
    // 析构的时候由于目标是nullptr所以自然结束掉
    SharedPtr(SharedPtr&& moving) noexcept : data(nullptr), PtrBase() {
        swap(moving);
        _destroy(moving);
    }
    
    // 移动赋值函数
    SharedPtr& operator=(SharedPtr&& moving) noexcept {
        if (this != &moving) {
            swap(moving);
            _destroy(moving);
        }
        return *this;
    }

    // 老三样
    pointer operator->() const noexcept { assert(data != nullptr); return data; }
    T& operator*() const noexcept { assert(data != nullptr); return *data; }
    pointer get() const noexcept { return data; }

    // 用于交换指针的成员函数
    void swap(SharedPtr& other) noexcept {
        std::swap(data, other.data);
        PtrBase::swap(other);
    }

    void swap(SharedPtr&& other) noexcept {
        std::swap(data, other.data);
        PtrBase::swap(other);
    }

    // 通过与新构造的对象交换来简化代码
    void reset(pointer p = nullptr) noexcept {
        swap(SharedPtr(p));
    }

    // 返回当前计数器次数
    int use_count() const noexcept { assert(counter != nullptr); return counter->uses; }

    explicit operator bool() const noexcept { return data != nullptr; }
};

#endif