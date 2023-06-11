#include<memory>
#include<iostream>
using namespace std;

class Object{
public:
    Object(){

    }
    ~Object()
    {
        cout<<"in ~Object()"<<endl;
    }
};

class ObjectTest
{
public:
    ObjectTest(int num)
    {
        ptr = std::shared_ptr<Object[]>(new Object[num], [](Object* data){delete[] data; });
    }
    ~ObjectTest()
    {

    }
private:
    shared_ptr<Object[]> ptr;
};

int main()
{
    ObjectTest t(10);
    return 0;
}