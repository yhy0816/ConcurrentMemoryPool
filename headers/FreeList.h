#pragma once


#include "Tools.h"
#include <cstddef>
#include <stdexcept>

// 自由链表个数
#define FREELIST_NUM 208
// ThreadCache单次申请最大字节数
#define MAX_BYTES 256 * 1024

// 管理切分好的小对象的自由链表
class FreeList {

public:

    static inline void*& NextObj(void* addr) {
        return *(void**)addr;
    }
    
    void push(void* obj);
    void* pop();
    
    inline bool Empty() {
        return _free_list_head == nullptr;
    }

private:
    void * _free_list_head = nullptr;

};


// 管理切分好的小对象的自由链表
template<std::size_t SIZE>
class FreeListSet {

public:

    // FreeList& operator[](size_t bytes);


    inline FreeList& Get(std::size_t idx) {
        if(idx >= SIZE) 
            throw std::invalid_argument("arg range error");
        return _freeLists[idx];
    }


private:
    FreeList _freeLists[SIZE];

};