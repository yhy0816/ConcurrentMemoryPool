#include "headers/ThreadCache.h"
#include "headers/AlignNumTools.h"
#include "headers/FreeList.h"


void* ThreadCache::Allocate(std::size_t bytes) {
    if(bytes > MAX_BYTES) 
            throw std::invalid_argument("arg range error");
    size_t index = AlignNumTools::Index(bytes);

    if(freeLists.Get(index).Empty()) {
        return FetchFromCentralCache(-1, -1); // TODO 暂时先这样
    } 
    
    return freeLists.Get(index).pop();    

}
void* ThreadCache::FetchFromCentralCache(std::size_t size, std::size_t alignSize) {
    // TODO ...
    return NULL;
}

void ThreadCache::Deallocate(void* obj, std::size_t size) {
    if(obj == nullptr) 
        throw std::invalid_argument("arg nullptr error");
    if(size > MAX_BYTES) 
        throw std::invalid_argument("arg range error");

    std::size_t index = AlignNumTools::Index(size); // 计算下标
    freeLists.Get(index).push(obj);  // 存入自由链表中

}