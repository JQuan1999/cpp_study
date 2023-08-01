// 装饰器模式：在不改变现有对象结构的情况下，动态的给对象增加功能的模式
// 装饰器是继承的有力补充，比继承灵活，在不改变原有对象的情况下，动态的给一个对象扩展功能，即插即用
// 通过使用不用装饰类及这些装饰类的排列组合，可以实现不同效果
// 装饰器模式完全遵守开闭原则
// 装饰器模式的组成：
// 1. 抽象构件(Component): 定义一个抽象接口以规范准备接受附加责任的对象
// 2. 具体构件(ConcreteComponent）角色：实现抽象构件，通过装饰角色为其添加一些职责
// 3. 抽象装饰（Decorator）角色：继承抽象构件，并包含具体构件的实例，可以通过其子类扩展具体构件的功能
// 4. 具体装饰（ConcreteDecorator）角色：实现抽象装饰的相关方法，并给具体构件对象添加附加的责任
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

// 抽象装饰类
class ShapeDecorator: public Shape
{
public:
    ShapeDecorator(Shape* shape)
    {
        _shape = shape;
    }
    virtual ~ShapeDecorator(){delete _shape; }
    virtual void draw()
    {
        _shape->draw();
    }
protected:
    Shape* _shape;
};

// 创建扩展了ShapeDecorator的实体装饰类
class RedShapeDecorator: public ShapeDecorator{
public:
    RedShapeDecorator(Shape* shape): ShapeDecorator(shape){}
    virtual ~RedShapeDecorator(){}
    virtual void draw()
    {
        _shape->draw();
        SetRedBorder(_shape);
    }
private:
    void SetRedBorder(Shape* shape)
    {
        cout<<"Border Color: Red. "<<endl;
    }
};

int main()
{
    RedShapeDecorator redRectangle(new Rectangle());
    redRectangle.draw();

    RedShapeDecorator redSquare(new Square());
    redSquare.draw();

    return 0;
}