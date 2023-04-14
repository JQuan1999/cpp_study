#include <iostream>
#include <memory>

class Widget{
public:
    Widget(){
        std::cout << "Widget constructor run" << std::endl;
    }
    ~Widget(){
        std::cout << "Widget destructor run" << std::endl;
    }

    std::shared_ptr<Widget> GetSharedObject(){
        return std::shared_ptr<Widget>(this);
    }
};

int main()
{
    try{
        std::shared_ptr<Widget> p(new Widget());
        // 在类内用this初始化一个智能指针对象 相当于用一个裸指针初始化两个智能指针会被释放两次
        std::shared_ptr<Widget> q = p->GetSharedObject();
        std::cout << p.use_count() << std::endl;
        std::cout << q.use_count() << std::endl;
    }
    catch(std::exception& e){
        std::cout << e.what() << '\n';
    }

    return 0;
}