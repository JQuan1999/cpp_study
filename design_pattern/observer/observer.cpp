// 观察者模式：它定义了一种一对多的依赖关系，当一个对象的状态发生改变时，其所有依赖者都会收到通知并自动更新。
// 意图：定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。
// 主要解决：一个对象状态改变给其他对象通知的问题，而且要考虑到易用和低耦合，保证高度的协作。
// 何时使用：一个对象（目标对象）的状态发生改变，所有的依赖对象（观察者对象）都将得到通知，进行广播通知。
// 如何解决：使用面向对象技术，可以将这种依赖关系弱化。
// 关键代码：在抽象类里有一个 ArrayList 存放观察者们。
// 核心角色：
// 主题（Subject）：也称为被观察者或可观察者，它是具有状态的对象，并维护着一个观察者列表。主题提供了添加、删除和通知观察者的方法。
// 观察者（Observer）：观察者是接收主题通知的对象。观察者需要实现一个更新方法，当收到主题的通知时，调用该方法进行更新操作。
// 具体主题（Concrete Subject）：具体主题是主题的具体实现类。它维护着观察者列表，并在状态发生改变时通知观察者。
// 具体观察者（Concrete Observer）：具体观察者是观察者的具体实现类。它实现了更新方法，定义了在收到主题通知时需要执行的具体操作。

// 观察者模式通过将主题和观察者解耦，实现了对象之间的松耦合。当主题的状态发生改变时，所有依赖于它的观察者都会收到通知并进行相应的更新。
#include<iostream>
#include<string>
#include<exception>
#include<memory>
#include<vector>
using namespace std;

class Subject;
class Observer{
protected:
    Subject* subject;
public:
    virtual ~Observer(){}
    virtual void update() = 0;
};

class Subject{
public:
    Subject(){}
    ~Subject(){}
    int GetState()
    {
        return state;
    }
    void SetState(int state)
    {
        this->state = state;
        NotifyAllObservers();
    }
    void Attach(Observer* obs)
    {
        observers.push_back(obs);
    }
    void NotifyAllObservers()
    {
        for(Observer* obs: observers)
        {
            obs->update();
        }
    }
private:
    int state;
    vector<Observer*> observers;
};

class BinaryObserver: public Observer{
public:
    virtual ~BinaryObserver(){};
    BinaryObserver(Subject* subject)
    {
        this->subject = subject;
        this->subject->Attach(this);
    }
    void update()
    {
        cout<<"Inside BinaryObserver::update(). "<<endl;
    }
};

class OctalObserver : public Observer{
public:
    virtual ~OctalObserver (){};
    OctalObserver (Subject* subject)
    {
        this->subject = subject;
        this->subject->Attach(this);
    }
    void update()
    {
        cout<<"Inside OctalObserver::update(). "<<endl;
    }
};

class HexaObserver  : public Observer{
public:
    virtual ~HexaObserver  (){};
    HexaObserver  (Subject* subject)
    {
        this->subject = subject;
        this->subject->Attach(this);
    }
    void update()
    {
        cout<<"Inside HexaObserver::update(). "<<endl;
    }
};

int main()
{
    Subject subject;
    BinaryObserver ob1(&subject);
    OctalObserver ob2(&subject);
    HexaObserver ob3(&subject);
    subject.SetState(2);
    return 0;
}