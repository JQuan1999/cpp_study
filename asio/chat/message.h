#ifndef MESSAGE
#define MESSAGE

#include<cstring>
#include<cassert>
#include<string>

enum{
    MAX_LENGTH = 1024
};

class Message{
public:
    Message(const Message& msg){
        memcpy(_data, msg._data, msg._cur_length);
        _cur_length = msg._cur_length;
    }

    // 接受消息的构造函数
    Message():_prefix_length(0), _cur_length(0)
    {

    }

    // 用户发送的消息的构造函数
    Message(const char* prefix, const char* sep = "---:"){
        memset(_data, 0, MAX_LENGTH);
        int p_len = strlen(prefix), s_len = strlen(sep);
        // msg组成: 前缀 + 分隔符 + 正文
        // 例如 Jack---:hello world
        strncpy(_data, prefix, p_len);
        strncpy(_data + p_len, sep, s_len);
        _prefix_length = p_len + s_len;
        _cur_length = _prefix_length;
    }
    
    void Append(const char* s, int size){
        assert(_cur_length + size <= _total_length);
        strncpy(Header(), s, size);
        write_n(size);
    }

    void Append(const std::string& s){
        Append(s.c_str(), s.size());
    }

    int Space(){
        return _total_length - _cur_length;
    }

    void Reset(){
        _cur_length = _prefix_length;
        memset(_data+_prefix_length, 0, _total_length - _prefix_length);
    }

    char* Data(){
        return _data;
    }

    char* Header(){
        return _data + _cur_length;
    }

    int MessageLength(){
        return _cur_length;
    }
    
    void write_n(int len){
        assert(_cur_length + len <= _total_length);
        _cur_length += len;
    }
private:
    char _data[MAX_LENGTH];
    static int _total_length;
    int _cur_length;
    int _prefix_length;
};

int Message::_total_length = MAX_LENGTH;

#endif