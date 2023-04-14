#include<iostream>
#include"my_sharePtr.h"

using std::cout;
using std::endl;

int main(){
    SharePtr<int> ptr1(new int(10));
    cout<<"ptr1 cout = "<<ptr1.use_count()<<endl;
    SharePtr<int> ptr2(ptr1);
    cout<<"after copy constructor\n";
    cout<<"ptr1 cout = "<<ptr1.use_count()<<endl;
    cout<<"ptr2 cout = "<<ptr2.use_count()<<endl;

    SharePtr<int> ptr3(std::move(ptr1));
    // ptr1的指针已经被ptr3偷走
    
    cout<<"ptr2 cout = "<<ptr2.use_count()<<endl;
    cout<<"ptr3 cout = "<<ptr3.use_count()<<endl;
    return 0;
}