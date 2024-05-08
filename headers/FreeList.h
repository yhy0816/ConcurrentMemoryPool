#pragma once

#include <assert.h>
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
    

private:
    void * _free_list_head = nullptr;

};


// 管理切分好的小对象的自由链表
template<std::size_t SIZE>
class FreeListSet {

public:

    FreeList& operator[](size_t bytes);

private:
    FreeList _freeLists[SIZE];

};