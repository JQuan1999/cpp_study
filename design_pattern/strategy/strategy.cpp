// 策略模式：一个类的行为或算法可以在运行时更改
// 策略模式定义了一系列算法或策略，将每个算法封装在独立的类中，使其可以相互替换。可以在运行时根据需要选择不同的算法，而不需要修改客户端代码
// 策略模式核心角色：
// 1. 环境：维护一个对策略对象的引用，负责将客户端请求委派给具体的策略对象执行。环境类可以通过依赖注入或简单工厂的方式获取
// 2. 抽象策略（Abstract Strategy）：定义了策略对象的公共接口或抽象类，规定了具体策略类必须实现的方法。
// 3. 具体策略（Concrete Strategy）：实现了抽象策略定义的接口或抽象类，包含了具体的算法实现。

// 主要解决：在有多种算法相似的情况下，使用 if...else 所带来的复杂和难以维护。
// 何时使用：一个系统有许多许多类，而区分它们的只是他们直接的行为。
// 如何解决：将这些算法封装成一个一个的类，任意地替换。
// 关键代码：实现同一个接口。

#include<iostream>
#include<string>
#include<exception>
#include<memory>
using namespace std;

class Strategy{
public:
    virtual ~Strategy(){}
    virtual int Operation(int num1, int num2) = 0;
};

class StrategyAdd: public Strategy{
public:
    virtual ~StrategyAdd(){}
    virtual int Operation(int num1, int num2)
    {
        return num1 + num2;
    }
};

class StrategySub: public Strategy{
public:
    virtual ~StrategySub(){}
    virtual int Operation(int num1, int num2)
    {
        return num1 - num2;
    }
};

class StrategyMul: public Strategy{
public:
    virtual ~StrategyMul(){}
    virtual int Operation(int num1, int num2)
    {
        return num1 * num2;
    }
};


// Context类
class Context{
public:
    Context(const shared_ptr<Strategy>& strategy)
    {
        this->strategy = strategy;
    }
    int executeStrategy(int n1, int n2)
    {
        return strategy->Operation(n1, n2);
    }
private:
    shared_ptr<Strategy> strategy;
};

int main()
{
    int  n1 = 10, n2 = 5;
    Context context1(make_shared<StrategyAdd>());
    cout<<"StrategyAdd result: "<<context1.executeStrategy(n1, n2)<<endl;

    Context context2(make_shared<StrategySub>());
    cout<<"StrategySub result: "<<context2.executeStrategy(n1, n2)<<endl;

    Context context3(make_shared<StrategyMul>());
    cout<<"StrategyMul result: "<<context3.executeStrategy(n1, n2)<<endl;

    return 0;
}