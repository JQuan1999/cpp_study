#ifndef WEAKPTR
#define WEAKPTR
#include"PtrBase.h"
#include"SharePtr.h"

template<class T>
class WeakPtr : public PtrBase {
    using pointer = T*;
    friend class SharedPtr<T>;

private:
    pointer data;

private:
    void _destroy(WeakPtr& target) noexcept {
        if (data != nullptr) {
            target.reduce_weak_count();
            target.check_mutex();
        }
    }

public:
    // 用于外部获取指针原始类型
    typedef T ElementType;
    WeakPtr() noexcept : data(nullptr), PtrBase() {}
    ~WeakPtr() noexcept { _destroy(*this); }
    WeakPtr(const SharedPtr<T>& sptr) noexcept :data(sptr.data), PtrBase(sptr.counter, sptr.mutex) {
        if (data != nullptr) {
            increase_weak_count();
        }
    }
    WeakPtr& operator=(const SharedPtr<T>& copy) noexcept {
        WeakPtr<T> tmp(copy);
        swap(tmp);
        return *this;
    }
    WeakPtr(const WeakPtr& copy) noexcept :data(copy.data), PtrBase(copy.counter, copy.mutex) {
        if (data != nullptr) {
            increase_weak_count();
        }
    }
    WeakPtr& operator=(const WeakPtr& copy) noexcept {
        WeakPtr tmp(copy);
        swap(tmp);
        if (data != nullptr) {
            increase_weak_count();
        }
        return *this;
    }
    WeakPtr& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    WeakPtr(WeakPtr&& moving) noexcept : data(nullptr), PtrBase() {
        swap(moving);
        _destroy(moving);
    }

    WeakPtr& operator=(WeakPtr&& moving) noexcept {
        if (this != &moving) {
            swap(moving);
            _destroy(moving);
        }
        return *this;
    }

    SharedPtr<T> lock() noexcept {
        if ( expired() ) {
            return SharedPtr<T>(nullptr);;
        }
        else {
            return SharedPtr<T>(*this);
        }
    }

    void reset() noexcept {
        swap(WeakPtr());
    }

    void swap(WeakPtr& other) noexcept {
        std::swap(data, other.data);
        PtrBase::swap(other);
    }

    void swap(WeakPtr&& other) noexcept {
        std::swap(data, other.data);
        PtrBase::swap(other);
    }

    int use_count() const noexcept { assert(counter != nullptr);  return counter->uses; }
    bool expired() const noexcept { return counter->uses == 0; }
};

#endif