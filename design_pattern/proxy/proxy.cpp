// 代理模式：一个类代表另一个类的功能。为其他对象提供一种代理以控制对这个对象的访问
// 何时使用：想在访问一个类时做一些控制。实现：增加中间层
// 适配器模式的区别：适配器模式主要改变所考虑对象的接口，而代理模式不能改变所代理类的接口。 
// 2、和装饰器模式的区别：装饰器模式为了增强功能，而代理模式是为了加以控制。
#include<iostream>
#include<string>
#include<exception>
#include<memory>
using namespace std;

// 创建接口
class Image{
public:
    virtual ~Image() {};
    virtual void display() = 0;
};

// 创建实现接口的实体类
class RealImage: public Image{
public:
    virtual ~RealImage(){}
    RealImage(string filename)
    {
        this->fileName = filename;
    }
    virtual void display()
    {
        cout<<"Displaying "<<fileName<<endl;
    }
private:
    string fileName;
};

// 代理类
class ProxyImage: public Image{
public:
    ProxyImage(string filename)
        : realImage(nullptr)
        , filename(filename)
    {
        
    }

    virtual ~ProxyImage()
    {

    }
    void display()
    {
        if(!realImage.get()){
            realImage = make_shared<RealImage>(filename);
        }
        PreProcess(); // 预处理
        realImage->display(); // 调用被代理类的方法
        PostProcess(); // 后处理
    }
private:
    void PreProcess()
    {
        cout<<"Inside PreProcess. "<<endl;
    }
    void PostProcess()
    {
        cout<<"Inside PostProcess. "<<endl;
    }
private:
    // RealImage* realImage;
    shared_ptr<RealImage> realImage;
    string filename;
};

int main()
{
    Image* proxy_image = new ProxyImage("proxy_image.jpg");
    proxy_image->display();
    delete proxy_image;
    return 0;
}