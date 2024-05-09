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

    inline size_t& get_maxSize(){
        return max_size;
    }
    //将start -> end 头插到自由链表中
    void PushRange(void* start, void* end);

    //将前popNum个节点摘除返回, 若当前链表长度不足popNum，则全部返回
    size_t PopRange(size_t popNum, void*& start, void*& end);


private:
    void * _free_list_head = nullptr; // 自由链表
    size_t max_size = 1; //当前自由链表申请未达到上限时，能申请的最大块空间，有上限
};


