#include<iostream>
#include<functional>
#include<algorithm>
#include<memory>
#include<cstdio>
using namespace std;

class Test{
public:
    Test(int a = 1, int b = 1): _a(a), _b(b){};
    Test(const Test&) = delete;
    Test(Test&& _t){
        cout<<"in function "<<__FUNCTION__<<endl;
        swap(_a, _t._a);
        swap(_b, _t._b);
    }
int _a, _b;
};

// 传递t的引用可以解决问题
void fun1(Test& t){
    cout<<"a + b = "<<t._a + t._b<<endl;
}

// 测试给bind函数传入一个禁止拷贝的参数
void test1(){
    Test t(1, 1);
    auto f = std::bind(fun1, std::move(t));
    f();
}

class Base{
public:
    Base(){};
    virtual ~Base(){
        cout<<"Base destructor "<<endl;
    }
    virtual void print(){
        cout<<"base print()"<<endl;
    }
};


class Derived: public Base{
public:
    Derived(){};
    virtual ~Derived(){
        cout<<" Derived destructor "<<endl;
    }
    virtual void print(){
        cout<<" Derived print()"<<endl;
    }
};


// 测试2: 子类智能指针赋值给基类智能指针
void test2(){
    shared_ptr<Base> ptr = make_shared<Derived>();
    ptr->print();
}

int main(){
    test1();
    return 0;
}