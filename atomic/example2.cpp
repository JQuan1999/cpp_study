#include<atomic>
#include<thread>
#include<iostream>
using namespace std;

template<typename T>
struct Node
{
    T data;
    Node* next;
    Node(const T& data = -1):data(data), next(nullptr){}
    ~Node(){
        std::cout<<"Node is deconstructed, data = "<<data<<endl;
    }
};

// 原子变量实现多线程插入元素
template<typename T>
class Stack{
private:
    enum STATUS{
        FREE = 0,
        LOCK = 1
    };
    STATUS free;
    STATUS lock;
    atomic<STATUS> flag;
    Node<T>* head;
public:
    Stack()
    {
        head == nullptr;
        free = FREE;
        lock = LOCK;
        flag.store(free);
    }
    ~Stack()
    {  
        Node<T>* cur = head, *next;
        while(cur)
        {
            next = cur->next;
            delete cur;
            cur = next;
        }
    }
    Node<T>* Top()
    {
        return head;
    }
    void Push(const T& data)
    {
        // 如果原子变量为free则更新为LOCK返回true 否则返回false
        while(!flag.compare_exchange_strong(free, lock));
        Node<T>* tmp = new Node<T>(data);
        if(!head){
            head = tmp;
        }else{
            tmp->next = head;
            head = tmp;
        }
        cout<<"thread: "<<this_thread::get_id()<<" push data "<<data<<" into stack"<<endl;
        // 重新恢复free
        flag.store(free);
        return;
    }
};


Stack<int> s;

void PushThread(int begin, int add, int end)
{
    for(; begin < end; begin += add)
    {
        s.Push(begin);
    }
}

int main()
{
    thread t1 = thread(PushThread, 0, 2, 100);
    thread t2 = thread(PushThread, 1, 2, 100);
    t1.join();
    t2.join();
    Node<int>* cur = s.Top();
    while(cur){
        std::cout<<cur->data<<" ";
        cur = cur->next;
    }
    cout<<endl;
    return 0;
}