#include<iostream>
#include<mutex>

using std::cout;
using std::endl;

#define LAZY_MODE2

#ifdef HUNGRY_MODE
// 饿汉模式
class single
{
private:
    static single* p;
    // 私有化构造函数和析构函数
    single(){}
    ~single(){}  
public:
    static single* getInstance();
};
single* single::p = new single();
single* single::getInstance()
{
    return p;
}
#endif


#ifdef LAZY_MODE1
class single
{
private:
    // 私有静态指针指向唯一实例 
    static single* p;
    static std::mutex m_mutex;
    single(){}
    ~single(){}
public:
    static single* getInstance();
};

std::mutex single::m_mutex;
single* single::p = nullptr;

single* single::getInstance()
{
    if(nullptr == p)
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        if(nullptr == p)
        {
            p = new single();
        }
    }
    return p;
}

#endif

// 使用函数内的局部静态对象
#ifdef LAZY_MODE2

class single
{
private:
    single(){};
    ~single(){};
public:
    static single* getInstance();
};

single* single::getInstance()
{
    static single obj;
    return &obj;
}
#endif

int main()
{
    single* p1 = single::getInstance();
    single* p2 = single::getInstance();
    if(p1 == p2)
    {
        cout<<"p1 is same and p2"<<endl;
    }
    return 0;
}