#ifndef _A_H
#define _A_H
#include<memory>

class B;

struct options{
    int _a, _b;
    options(int a, int b):_a(a), _b(b){}
};

class A{
public:
    A();
    void print();
private:
    std::shared_ptr<B> b_ptr;
};

#endif