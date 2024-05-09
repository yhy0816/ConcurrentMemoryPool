#include "headers/ThreadCache.h"
#include "headers/AlignNum.h"
#include "headers/FreeList.h"
#include <iostream>

// const std::size_t ThreadCache::MAX_BYTES = 256 * 1024;

void* ThreadCache::Allocate(std::size_t bytes) {
    if(bytes > MAX_BYTES) 
            throw std::invalid_argument("arg range error");
    size_t index = AlignNum::Index(bytes);

    if(freeLists[index].Empty()) {
        return FetchFromCentralCache(-1, -1); // TODO 暂时先这样
    } 
    
    return freeLists[index].pop();    

}
void* ThreadCache::FetchFromCentralCache(std::size_t size, std::size_t alignSize) {
    // TODO ...

    // std::cout << "size : " << size << "alignSize: " << alignSize << std::endl;
    return NULL;
}

void ThreadCache::Deallocate(void* obj, std::size_t size) {
    if(obj == nullptr) 
        throw std::invalid_argument("arg nullptr error");
    if(size > MAX_BYTES) 
        throw std::invalid_argument("arg range error");

    std::size_t index = AlignNum::Index(size); // 计算下标
    freeLists[index].push(obj);  // 存入自由链表中

}

__thread  ThreadCache TLSThreadCache = ThreadCache();