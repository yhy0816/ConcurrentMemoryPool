#include "headers/CentralCache.h"

CentralCache CentralCache::centralCache = CentralCache(); 


Span* getASpan(size_t index) {
    //TODO ...
    return nullptr;
}

size_t CentralCache::fetchRangeObj(size_t index, size_t batchNum, void*& start, void*& end) {
    // CentralCache是所有线程共享的所以访问对应的桶要加锁
    spanLists[index].mtx.lock();

    Span* span = getASpan(-1); //TODO 这里要改

    size_t actualNum = span->freeList.PopRange(batchNum, start, end);

    spanLists[index].mtx.unlock();

    return actualNum;
    //TODO
}