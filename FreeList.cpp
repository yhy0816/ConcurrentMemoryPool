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



// // 根据bytes 作为索引 直接返回对应桶的引用
// template<std::size_t SIZE>
// FreeList& FreeListSet<SIZE>::operator[](size_t bytes) {
//     return this->_freeLists[AlignNumTools::Index(bytes)];
// }