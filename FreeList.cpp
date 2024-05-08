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

static size_t GetAlignNum(std::size_t bytes) {
    if(bytes <= 128) { // 8B 对齐

        return 8;

    } else if (bytes <= 1024){ // 16B

        return 16;

    } else if (bytes <= 8 * 1024) { // 128B

        return 128;

    } else if (bytes <= 64 * 1024) { // 1024B

        return 1024;

    } else if(bytes <= 256 * 1024) { // 8 * 1024B 对齐

        return 8 * 1024;
    } 
    throw std::invalid_argument("arg range error");
}


    // 计算对齐后的字节数
size_t RoundUp(std::size_t bytes) {
    
    std::size_t alignNum = GetAlignNum(bytes);
    if(bytes % alignNum == 0) return bytes;
    return (bytes / alignNum + 1) * alignNum;
    

}


static std::size_t IndexHandle(std::size_t bytes) {
    if(bytes <= 128) { // 8B 对齐

        return 0;

    } else if (bytes <= 1024){ // 16B

        return 1;

    } else if (bytes <= 8 * 1024) { // 128B

        return 2;

    } else if (bytes <= 64 * 1024) { // 1024B

        return 3;

    } else if(bytes <= 256 * 1024) { // 8 * 1024B 对齐

        return 4;
    } 
    throw std::invalid_argument("arg range error");
}


//计算对应的字节数应该放在哪个桶里
size_t Index(std::size_t bytes) {
    if(bytes > MAX_BYTES) 
        throw std::invalid_argument("arg range error");

    const int bases[5] = { 0, 16, 72, 128, 184};

    std::size_t alignNum = GetAlignNum(bytes);
    int off;
    if(bytes % alignNum == 0) off = (bytes - 1) / alignNum;
    else off = bytes / alignNum + 1; // 计算 这个字节数 相对于它开始第一个桶的偏移量

    return bases[IndexHandle(bytes)] + off;
    
}

// 根据bytes 作为索引 直接返回对应桶的引用
template<std::size_t SIZE>
FreeList& FreeListSet<SIZE>::operator[](size_t bytes) {
    return this->_freeLists[Index(bytes)];
}