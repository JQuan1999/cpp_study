#include<string>
#include<iostream>
#include<deque>
using namespace std;

int main()
{
    deque<string> q;
    while(q.size() < 5)
    {
        string s;
        s.reserve(128);
        cout<<"s.size(): "<<s.size()<<"s.capacity(): "<<s.capacity()<<endl;
        q.push_back(s);
    }
    for(auto& s: q){
        cout<<"s.size(): "<<s.size()<<"s.capacity(): "<<s.capacity()<<endl;
    }
    return 0;
}