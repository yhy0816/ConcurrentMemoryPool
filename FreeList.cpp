#include "headers/FreeList.h"



// 将被释放的对象头插到自由链表中
void FreeList::push(void* obj) {
    if(obj == nullptr) 
        throw std::invalid_argument("arg is null");
    NextObj(obj) = this->_free_list_head;
    _free_list_head = obj;
}

// 将自由链表头节点分配出去
void* FreeList::pop() {

    if(_free_list_head == nullptr) 
        throw std::invalid_argument("arg is null");
    void* obj = _free_list_head;
    _free_list_head = NextObj(obj);
    return obj;
}

void FreeList::PushRange(void* start, void* end) {
    NextObj(end) = _free_list_head;
    _free_list_head = start;
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
    return count;
}

// // 根据bytes 作为索引 直接返回对应桶的引用
// template<std::size_t SIZE>
// FreeList& FreeListSet<SIZE>::operator[](size_t bytes) {
//     return this->_freeLists[AlignNumTools::Index(bytes)];
// }