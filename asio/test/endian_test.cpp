#include<iostream>
#include<cstdlib>
#include<cstring>

using namespace std;
/* 测试字节序 小端存储 */
int main()
{
    char nums[4] = {0};
    int a = 128;
    strncpy(nums, reinterpret_cast<char*>(&a), 4);
    std::cout<<"nums = "<<nums<<std::endl;
    int b;
    strncpy(reinterpret_cast<char*>(&b), nums, 4);
    std::cout<<"after change b = "<<b<<std::endl;
    return 0;
}