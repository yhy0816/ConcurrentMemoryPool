#include "headers/FreeList.h"

#include <iostream>

// 将被释放的对象头插到自由链表中
void FreeList::push(void* obj) {
    if(obj == nullptr) 
        throw std::invalid_argument("arg is null");
    NextObj(obj) = this->_free_list_head;
    _free_list_head = obj;
    size++;
}

// 将自由链表头节点分配出去
void* FreeList::pop() {

    if(_free_list_head == nullptr) 
        throw std::invalid_argument("arg is null");
    void* obj = _free_list_head;
    _free_list_head = NextObj(obj);
    size--;
    return obj;
}

void FreeList::PushRange(size_t batchNum, void* start, void* end) {
    NextObj(end) = _free_list_head;
    _free_list_head = start;
    size += batchNum;

}

size_t FreeList::PopRange(size_t popNum, void*& start, void*& end) {
    // if(_free_list_head == nullptr) 
    
    if(_free_list_head == nullptr) return 0; // 没有节点直接返回
    start = _free_list_head;
    end = _free_list_head;
    size_t count = 1;
    for(int i = 1; i < popNum ; i++) {
        void* next = NextObj(end);
        if(next == nullptr) break;
        end = next;
        count++;
    }
    
    // 获取前popNum个节点，或节点数不足获取全部节点
    _free_list_head = NextObj(end);
    NextObj(end) = nullptr;
    size -= count;
    return count;
}

void FreeList::PushBlock(size_t blockSize, void* start, void* end) {
    if(start >= end) {
        throw std::invalid_argument("PushBlock arg err");
    }
    // 接下来 要把这份页空间切分成size大小的链表的形式

    char* cur = (char*)start;
    void* tail = start;
    cur += blockSize;
    size_t batchNum = 1;

    while(cur < end) {
        batchNum++;
        FreeList::NextObj(tail) = cur;
        cur += blockSize;
        tail = FreeList::NextObj(tail);
        // std::cout << 1 << std::endl;
    }
    FreeList::NextObj(tail) = nullptr;

    PushRange(batchNum, start, tail);

}