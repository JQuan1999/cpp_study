// 单例模式(signleton pattern)
// 该模式负责创建自己的对象，同时确保只有单个对象被创建。确保一个类只有一个实例，并提供了一个全局访问点来访问该实例
// 在实现单例模式时，要考虑线程安全的问题
// 单例模式可以分为懒汉式和饿汉式：
// 1. 懒汉式：运行时实例并不存在，只有当需要使用该实例的时候，才会创建并使用该实例。要考虑线程安全
// 2. 饿汉式：系统一运行就初始化实例，当需要时直接调用即可。不需要考虑线程安全
#include<iostream>
#include<memory>
#include<mutex>
using namespace std;

// 懒汉式 延迟初始化
class SingleObject1{
public:
    static SingleObject1* getInstance()
    {
        // 1. 线程不安全
        // if(!instance.get()){
        //     instance = make_shared<SingleObject1>();
        // }

        // 2. 加锁
        // lock_guard<mutex> lock(m);
        // if(!instance.get()){
        //     instance = make_shared<SingleObject1>();
        // }

        // 3. 双重检查 因为只有在第一次初始化的过程中会存在线程安全问题，在后面获取实例的时候并不会遇到也就没必要使用lock
        // 双重检查，通过加锁前先检查是否已经初始化，避免每次获取实例的时候都要首先获取锁
        // if(!instance.get()){
        //     lock_guard<mutex> lock(m);
        //     if(!instance.get()){
        //         instance = make_shared<SingleObject1>();
        //     }
        // }
        // return instance.get();
        
        // 4. 获取静态局部对象
        // 使用函数内部的静态局部对象，只有当第一次访问getInstance的时候才创建实例
        static SingleObject1 obj;
        return &obj;
    }
    void Show()
    {
        cout<<"Inside SingleObject1::Show(). "<<endl;
    }
private:
    // 将构造函数设为私有
    SingleObject1(){}
    ~SingleObject1(){}
    SingleObject1(const SingleObject1&){};
    SingleObject1& operator=(const SingleObject1&);
private:
    static mutex m;
    static shared_ptr<SingleObject1> instance;
};

// 饿汉式
class SingleObject2{
private:
    static SingleObject2 instance;
private:
    // 将构造函数设为私有
    SingleObject2(){}
    ~SingleObject2(){}
    SingleObject2(const SingleObject2&){};
    SingleObject2& operator=(const SingleObject2&);
public:
    static SingleObject2* getInstance()
    {
        return &instance;
    }
    void Show()
    {
        cout<<"Inside SingleObject2::Show(). "<<endl;
    }
};

SingleObject2 SingleObject2::instance; // 在main函数之前初始化

int main()
{
    SingleObject1::getInstance()->Show();
    SingleObject2::getInstance()->Show();
    return 0;
}
