#ifndef _B_H
#define _B_H

#include"A.h"
#include<iostream>

class B{
public:
    B(int a, int b): _opt(a, b){

    }

    void print()
    {
        std::cout<<"a = "<<_opt._a<<" b = "<<_opt._b<<std::endl;
    }
private:
    options _opt;
};

#endif