#include<stdarg.h>
#include<cstdio>
#include<cstring>
#include<iostream>
using std::cout;
using std::endl;

// 求n个数的和
void SumN(int size, ...)
{
    va_list vp; 
    va_start(vp, size); // 取最后一个参数的地址 栈空间从高到低 因此第一参数的地址最高
    int sum = 0, i = 0;
    cout<<"vp = "<<vp<<endl;
    for(; i < size; i++){
        int tmp = va_arg(vp, int); // 获取当前参数 并将指针移动到下一参数
        printf("tmp = %d\n", tmp);
        cout<<"vp = "<<vp<<endl;
    }
    va_end(vp); // 将vp置为null
    cout<<"vp = "<<vp<<endl;
}

void Print(const char* fmt, ...){
    char buf[1024];
    // char buf2[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 1024, fmt, ap);
    // snprintf(buf, 1024, fmt, ap);
    printf("buf is %s\n", buf);
    // printf("buf2 is %s\n", buf2);
    va_end(ap);
}


void foo(char *fmt, ...)
{
	va_list ap;
	int d ;
	char c, *s;
 
	va_start(ap, fmt);
	while(*fmt)
	{
		switch(*fmt++){
			case 'S':
			case 's':
				s = va_arg(ap, char*);
				printf("string is %s\n", s);
				break;
			case 'd':
				d = va_arg(ap, int);
				printf("type int data is %d\n", d);
				break;
			case 'c':
				c = (char)va_arg(ap, int);
				printf("type char data is %c\n", c);
				break;
 
			}
	va_end(ap);
 
	}
}

int main()
{
    foo("hello %s %d %c", "123", 10, 'c');
    return 0;
}