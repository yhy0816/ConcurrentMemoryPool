#pragma once


#include "Tools.h"
#include <cstddef>
#include <stdexcept>

// 自由链表个数
#define FREELIST_NUM (208)


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


