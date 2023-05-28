#include<atomic>
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

template<typename T>
class Stack{
private:
    std::atomic<Node<T>*> head;
public:
    Stack()
    {
        Node<T>* h = new Node<T>();
        head.store(h);
    }
    ~Stack()
    {
        Node<T>* cur = head.load(), *next;
        while(cur)
        {
            next = cur->next;
            delete cur;
            cur = next;
        }
    }
    void Push(const T& data)
    {
        Node<T>* new_node = new Node<T>(data);
        new_node->next = head.load(std::memory_order_relaxed);
        // 将head更新为new_node
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }
};

int main()
{
    Stack<int> s;
    for(int i = 0; i < 5; i++){
        s.Push(i);
    }
}