#include"test.pb.h"
#include"zero_copy_stream.h"
#include<iostream>
using namespace std;

int main()
{
    test::Hello hello, message;
    hello.set_name("jq");
    hello.set_age(23);
    hello.set_id(2);
    std::string str;
    hello.SerializeToString(&str);
    cout<<"hello.size(): "<<str.size()<<endl;
    string part1(str.begin(), str.begin()+4);
    string part2(str.begin()+4, str.end());
    cout<<"part1.size(): "<<part1.size()<<endl;
    cout<<"part2.size(): "<<part2.size()<<endl;

    ReadBuffer zero_string;
    zero_string.Append(part1);
    zero_string.Append(part2);
    if(message.ParseFromZeroCopyStream(&zero_string))
    {
        std::cout<<"after parse: "<<std::endl;
        std::cout<<"name = "<<message.name()<<std::endl;
        std::cout<<"id = "<<message.id()<<std::endl;
        std::cout<<"age = "<<message.age()<<std::endl;
    }
    else{
        std::cout<<"parse error "<<std::endl;
    }
    return 0;
}