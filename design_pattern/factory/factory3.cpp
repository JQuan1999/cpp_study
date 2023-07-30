// 抽象工厂模式：把有关联关系的，属于一个产品簇的所有产品创建的接口函数，放在一个抽象工厂里面AbstractFactory，派生类（具体产品的工厂）应该负责创建该产品簇里面所有的产品
// 抽象工厂模式是围绕一个超级工厂创建其他工厂。该超级工厂又称为其他工厂的工厂
#include<iostream>
#include<string>
#include<exception>
#include<memory>
using namespace std;

// 形状接口
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

// 颜色接口
class Color{
public:
    virtual ~Color(){};
    virtual void fill() = 0;
};

class Red: public Color{
public:
    virtual ~Red(){}
    virtual void fill()
    {
        cout<<"Inside Red::fill(). "<<endl;
    }
};

class Green: public Color{
public:
    virtual ~Green(){}
    virtual void fill()
    {
        cout<<"Inside Green::fill(). "<<endl;
    }
};

enum ColorType{
    RED = 0,
    GREEN = 1
};

// 为Shape和Color创建抽象类来获取工厂
class AbstractFactory{
public:
    virtual ~AbstractFactory(){};
    virtual Shape* GetShape(ShapeType shapeType) = 0;
    virtual Color* GetColor(ColorType colorType) = 0;
};

class ShapeFactory: public AbstractFactory{
public:
    virtual ~ShapeFactory(){};
    virtual Shape* GetShape(ShapeType  shapeType){
        switch (shapeType)
        {
        case CIRCLE:
            return new Circle();
        case RECTANGLE:
            return new Rectangle();
        case SQUARE:
            return new Square();
        default:
            return nullptr;
        }
    }
    virtual Color* GetColor(ColorType colorType){
        return nullptr;
    }
};

class ColorFactory: public AbstractFactory{
public:
    virtual ~ColorFactory(){};
    virtual Shape* GetShape(ShapeType  shapeType){
        return nullptr;
    }
    virtual Color* GetColor(ColorType colorType){
        switch (colorType)
        {
        case RED:
            return new Red();
        case GREEN:
            return new Green();
        default:
            return nullptr;
        }
    }
};

enum FactoryType{
    SHAPE = 0,
    COLOR = 1
};

class FactoryProducer
{
public:
    static AbstractFactory* GetFactory(FactoryType type){
        switch (type)
        {
        case SHAPE:
            return new ShapeFactory();
        case COLOR:
            return new ColorFactory();
        default:
            return nullptr;
        }
    }
};

int main(){
    // 形状工厂
    shared_ptr<AbstractFactory> factory1(FactoryProducer::GetFactory(SHAPE));
    shared_ptr<Shape> shape1(factory1->GetShape(CIRCLE));
    shape1->draw();
    shared_ptr<Color> color1(factory1->GetColor(RED));
    if(color1.get() == nullptr){
        cout<<"color1 is nullptr"<<endl;
    }else{
        color1->fill();
    }
    shared_ptr<Shape> shape2(factory1->GetShape(RECTANGLE));
    shape2->draw();
    cout<<"----------------------"<<endl;
    // 颜色工厂
    shared_ptr<AbstractFactory> factory2(FactoryProducer::GetFactory(COLOR));
    shared_ptr<Color> color2(factory2->GetColor(RED));
    color2->fill();
    shared_ptr<Color> color3(factory2->GetColor(GREEN));
    color3->fill();

    return 0;
}