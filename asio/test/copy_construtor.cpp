#include<iostream>
#include<memory>
class Test{
public:
    Test(){};
    ~Test()
    {
        std::cout<<"in ~Test()"<<std::endl;
    }
    Test(const Test& t)
    {
        std::cout<<"in Test(const Test& t)"<<std::endl;
    }
};

void test()
{
    
}

int main()
{
    test();
    return 0;
}