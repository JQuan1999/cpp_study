#include"zero_copy_stream.h"
#include<iostream>
#include<fstream>
using namespace std;

/*
测试ZeroCopyOutStream 将out_size字节写入out
*/
void test1()
{
    WriteBuffer out;
    void* data;
    int size;
    string s = "adbsbdwe:";
    int out_size = 100, len = s.size(), write_size, offset;
    while(out_size > 0)
    {
        out.Next(&data, &size);
        offset = 0;
        cout<<"可写入的内存size: "<<size<<endl;
        while(size > 0 && out_size > 0){
            write_size = min(min(size, len), out_size);
            strncpy((char*)data+offset, s.c_str(), write_size);
            size -= write_size;
            out_size -= write_size;
            offset += write_size;
        }
        if(size > 0)
        {
            out.BackUp(size); // 归还多余的字节数
        }
    }
}

void test2()
{
    WriteBuffer out;
    void* data;
    int size;
    // 将input的文件写入out
    ifstream input("./zero_copy_stream.h", ios_base::in);
    while(!input.eof() && !input.bad())
    {
        out.Next(&data, &size);
        input.read((char*)data, size);
        int read_count = input.gcount();
        if(read_count < size)
        {
            out.BackUp(size - read_count);
        }
    }
    string ret = out.ToString();
    cout<<"final ret: ";
    cout<<ret<<endl;
    cout<<"out write bytes is: "<<out.ByteCount()<<endl;
    cout<<"out total bytes is: "<<out.TotalBytes()<<endl;
    cout<<"out block size is: "<<out.BlockCount()<<endl;
}

void test3()
{
    WriteBuffer out;
    void* data;
    int size;
    // 将input的文件写入out
    ifstream input("./client.cc", ios_base::in);
    while(!input.eof() && !input.bad())
    {
        out.Next(&data, &size);
        input.read((char*)data, size); // 读数据写入data处
        int read_count = input.gcount();
        if(read_count < size)
        {
            out.BackUp(size - read_count);
        }
    }
    string ret = out.ToString();
    cout<<"final ret: "<<ret<<endl;

    input.close();
    ReadBuffer readbuf;

    cout<<"test swapout: "<<endl;
    out.SwapOut(&readbuf);
    cout<<"readbuf data: "<<endl;
    cout<<readbuf;
    const void* read_data;
    while(readbuf.Next(&read_data, &size))
    {
        cout.write((char*)read_data, size);
    }
}

int main()
{
    test3();
}