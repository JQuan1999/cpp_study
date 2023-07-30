/*
工厂模式：创建对象的最佳方式，将对象的创建和使用代码分离，提供统一的接口来创建不同的对象.
在创建对象时不会对客户端暴露创建逻辑，而是通过一个共同的接口来指向新创建的对象
核心角色：1. 抽象产品(Abstract Product)， 定义了产品的共同接口或抽象类
2. 具体产品(Concrete Product)，实现了抽象产品接口，定义了具体产品的行为和属性
3. 抽象工厂(Abstract Factory)，声明了创建产品的抽象方法，可以拥有多个方法用于创建不同类型的产品
4. 具体工厂：实现了抽象工厂接口，负责创建具体产品对象
*/

#include<iostream>
#include<string>
#include<exception>
#include<memory>
using namespace std;

class Shape{
public:
    virtual void draw() = 0;
    virtual ~Shape(){
        cout<<"Inside Shape::~Shape()"<<endl;
    }
};

class Rectangle: public Shape{
public:
    virtual void draw(){
        cout<<"Inside Rectangle::draw() method."<<endl;
    }
    virtual ~Rectangle(){
        cout<<"Inside Rectangle::~Rectangle()"<<endl;
    }
};

class Square: public Shape{
public:
    virtual void draw(){
        cout<<"Inside Square::draw() method."<<endl;
    }
    virtual ~Square(){
        cout<<"Inside Square::~Square()"<<endl;
    }
};

class Circle: public Shape{
public:
    virtual void draw(){
        cout<<"Inside Circle::draw() method."<<endl;
    }
    virtual ~Circle(){
        cout<<"Inside Circle::~Circle()"<<endl;
    }
};

enum ShapeType{
    CIRCLE = 0,
    RECTANGLE,
    SQUARE
};

// 工厂方法(Factory Method)
// 定义一个Factory基类，基类中定义了一个纯虚函数(创建产品)
// 之后定义派生类（具体产品的工厂）负责创建对应的产品。
// 可以做到不同的产品在不同的工厂里面创建，能够对现有工厂以及产品的修改关闭。
class ShapeFactory{
public:
    virtual Shape* GetSetShape() = 0;
};

class RectangleFactory: public ShapeFactory
{
public:
    virtual Shape* GetSetShape(){
        return new Rectangle();
    }
};

class SquareFactory: public ShapeFactory
{
public:
    virtual Shape* GetSetShape(){
        return new Square();
    }
};

class CircleFactory: public ShapeFactory
{
public:
    virtual Shape* GetSetShape(){
        return new Circle();
    }
};

int main()
{
    // 创建RectangleFactory 然后调用GetShape方法
    // 在工厂模式提供统一的接口，传递创建的类型参数可以获得对应的对象，判断逻辑在该工厂内部
    unique_ptr<ShapeFactory> factory1(new RectangleFactory());
    Shape* shape1 = factory1->GetSetShape();
    shape1->draw();
    delete shape1;

    unique_ptr<ShapeFactory> factory2(new SquareFactory());
    Shape* shape2 = factory2->GetSetShape();
    shape2->draw();
    delete shape2;

    unique_ptr<ShapeFactory> factory3(new CircleFactory());
    Shape* shape3 = factory3->GetSetShape();
    shape3->draw();
    delete shape3;

    return 0;
}