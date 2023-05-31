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

void Delete(Object* ptr)
{
    delete[] ptr;
}

int main()
{
    shared_ptr<Object[]> ptr(new Object[2], Delete);
    return 0;
}