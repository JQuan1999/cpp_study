#include<google/protobuf/io/zero_copy_stream.h>
#include<string>
#include<deque>
#include<iostream>

#define MAX_FACTOR_SIZE 9
#define BASE_FACTOR_SIZE 3
#define BUFFER_UNIT 64

class Buffer{
public:
    Buffer(int factor_size)
    {
        _capacity = (BUFFER_UNIT << factor_size);
        _size = 0;
        shared_buf_ptr = std::shared_ptr<char[]>(new char[_capacity+1], 
                    [](char* data)
                    {
                        delete[] data;
                        data = nullptr;
                    }
        );
        _data = shared_buf_ptr.get();
        memset(_data, 0, _capacity+1);
    }
    Buffer(const Buffer& buf)
    {
        _capacity = buf._capacity;
        _size = buf._size;
        shared_buf_ptr = buf.shared_buf_ptr;
        _data = buf._data;
    }
    ~Buffer()
    {
        _data = nullptr;
        shared_buf_ptr.reset();
    }

    char* GetData()
    {
        return _data;
    }

    char* GetHeader()
    {
        return _data + _size;
    }

    void SetSize(int size)
    {
        _size = size;
    }

    int GetSize()
    {
        return _size;
    }

    void Forward(int bytes)
    {
        _size += bytes;
        if(_size > _capacity){
            std::cout<<"_size: "<<_size<<" is over _capacity: "<<_capacity<<", have truncted to _capacity"<<std::endl;
            _size = _capacity;
        }
    }

    void Back(int bytes)
    {
        _size -= bytes;
        if(_size < 0){
            std::cout<<"_size must be greater than 0"<<std::endl;
            _size = 0;
        }
    }

    void SetCapacity(int cap)
    {
        if(cap > _capacity)
        {
            std::cout<<"new cap must less than old capacity"<<std::endl;
            return;
        }
        _capacity = cap;
        if(_size > _capacity)
        {
            _size = _capacity;
        }
    }

    int GetCapacity()
    {
        return _capacity;
    }

    int GetSpace()
    {
        return _capacity - _size;
    }

    friend std::ostream& operator<<(std::ostream& os, const Buffer& buf)
    {
        os.write(buf._data+buf._size, buf._capacity-buf._size);
        return os;
    }

private:
    char* _data;
    std::shared_ptr<char[]> shared_buf_ptr;
    int _capacity;
    int _size;
};


class ReadBuffer: public google::protobuf::io::ZeroCopyInputStream
{
public:
    ReadBuffer()
    {
        _read_bytes = 0;
        _last_bytes = 0;
        _total_bytes = 0;
        _cur_iter = _buf_list.begin();
    }

    void Append(Buffer& buf)
    {
        if(buf.GetCapacity() == 0)
        {
            return;
        }
        _buf_list.push_back(buf);
        _total_bytes += buf.GetCapacity();
        _cur_iter = _buf_list.begin();
    }

    bool Next(const void** data, int* size)
    {
        if(_cur_iter == _buf_list.end())
        {
            _last_bytes = 0;
            return false;
        }else
        {
            *data = _cur_iter->GetHeader(); // 读取的指针
            *size = _cur_iter->GetSpace(); // 可读的大小
            _cur_iter++;
            _last_bytes = *size;
            _read_bytes += _last_bytes;
            return true;
        }
    }

    void BackUp(int count)
    {
        if(_last_bytes <= 0)
        {
            std::cout<<"last_bytes is not greater than zero"<<std::endl;
            return;
        }
        if(count < 0)
        {
            std::cout<<"count is not greater than zero"<<std::endl;
            return;
        }
        _cur_iter--;
        _cur_iter->Back(count);
        _last_bytes = 0;
        _read_bytes -= count;
    }

    bool Skip(int count)
    {
        if(count < 0)
        {
            std::cout<<"count is not greater than zero"<<std::endl;
            return false;
        }
        const void* data;
        int size;
        while(count > 0 && Next(&data, &size))
        {
            if(size > count)
            {
                BackUp(size - count);
                size = count;
            }
            count -= size;
        }
        _read_bytes += count;
        _last_bytes = 0;
        return count == 0;
    }

    int64_t ByteCount() const
    {
        return _read_bytes;
    }

    friend std::ostream& operator<<(std::ostream& os, ReadBuffer& buf)
    {
        for(auto iter = buf._buf_list.begin(); iter != buf._buf_list.end(); iter++)
        {
            os<<*iter;
        }
        return os;
    }
private:
    std::deque<Buffer> _buf_list;
    std::deque<Buffer>::iterator _cur_iter;
    int _last_bytes; // 最后一次读取的字节数
    int _total_bytes; // 累计可读的字节数
    int64_t _read_bytes; // 已读的字节数
};

class WriteBuffer: public google::protobuf::io::ZeroCopyOutputStream
{
public:
    WriteBuffer()
    {
        _last_bytes = 0;
        _total_bytes = 0;
        _write_bytes = 0;
        _cur_iter = _buf_list.rend();
    }

    void SwapOut(ReadBuffer* readbuf)
    {
        while(!_buf_list.empty())
        {
            Buffer buf = _buf_list.front();
            _buf_list.pop_front();
            buf.SetCapacity(buf.GetSize()); // 已写入的size为可读的容量
            buf.SetSize(0); // 已读的size = 0
            readbuf->Append(buf);
        }
        _last_bytes = 0;
        _total_bytes = 0;
        _write_bytes = 0;
    }

    std::string ToString()
    {
        std::string ret;
        ret.reserve(_total_bytes);
        for(auto iter = _buf_list.begin(); iter != _buf_list.end(); iter++)
        {
            ret.append(iter->GetData(), iter->GetSize());
        }
        return ret;
    }

    bool Next(void** data, int* size)
    {
        if(_cur_iter == _buf_list.rend() || _cur_iter->GetSpace() == 0)
        {
            if(!Extend())
            {
                _last_bytes = 0;
                return false;
            }
        }
        *data = _cur_iter->GetHeader();
        *size = _cur_iter->GetSpace();
        _cur_iter->Forward(*size);
        _cur_iter++;
        _last_bytes = *size;
        _write_bytes += _last_bytes;
        return true;
    }

    bool Extend()
    {
        int factor_size = std::min((int)_buf_list.size()+BASE_FACTOR_SIZE, MAX_FACTOR_SIZE);
        _buf_list.push_back(Buffer(factor_size));
        _cur_iter = _buf_list.rbegin();
        _total_bytes += _cur_iter->GetCapacity();
        return true;
    }

    void BackUp(int count)
    {
        if(_last_bytes <= 0)
        {
            std::cout<<"last_bytes is not greater than zero"<<std::endl;
            return;
        }
        if(count < 0)
        {
            std::cout<<"count is not greater than zero"<<std::endl;
            return;
        }
        --_cur_iter;
        _cur_iter->Back(count);
        _last_bytes = 0;
        _write_bytes -= count;
    }

    int64_t ByteCount() const
    {
        return _write_bytes;
    }

    int TotalBytes()
    {
        return _total_bytes;
    }

    int BlockCount()
    {
        return _buf_list.size();
    }

private:
    std::deque<Buffer> _buf_list;
    std::deque<Buffer>::reverse_iterator _cur_iter;
    int _last_bytes;
    int _total_bytes;
    int64_t _write_bytes;
};