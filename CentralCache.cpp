#include "headers/CentralCache.h"
#include "headers/FreeList.h"
#include "headers/PageCache.h"
#include "headers/ThreadCache.h"
#include "headers/PageCache.h"
#include <iostream>
CentralCache CentralCache::centralCache = CentralCache(); 



//从spanLists[index] 中找到一个管理非空链表的span, 如果没有，就去下层申请
Span* CentralCache::getASpan(size_t index, size_t size) {
    //...
    Span* aSpan = spanLists[index].getANotNULLSpan();
    if(aSpan != nullptr) {
        return aSpan;
    }
    //走到这里还没返回的话就去申请了

    spanLists[index].mtx.unlock();

    size_t pageNum = PageCache::getPageNum(size, index);
    //执行NewSpan时要给整个pageCache加锁
    PageCache::getInstance()->mtx.lock();
    Span* newSpan = PageCache::getInstance()->NewSpan(pageNum);
    PageCache::getInstance()->mtx.unlock();



    char* start  = (char*)(newSpan->pageId << PAGE_SHIFT); // 这份页空间的首地址
    char* end  = (char*)(start + (newSpan->pageNum << PAGE_SHIFT)); // 这份页空间的尾地址

    // 接下来 要把这份页空间切分成size大小的链表的形式

    char* cur = start;
    void* tail = start;
    cur += size;
    // std::cout << newSpan->pageId << " " << newSpan->pageNum<< std::endl;
    while(tail < end) {
        FreeList::NextObj(tail) = cur;
        cur += size;
        tail = FreeList::NextObj(tail);
    }
    FreeList::NextObj(tail) = nullptr;
    newSpan->freeList.PushRange(start, tail);

    spanLists[index].mtx.lock();
    this->spanLists[index].PushFront(newSpan);
    return newSpan;
}

size_t CentralCache::fetchRangeObj(size_t index, size_t size, size_t batchNum, void*& start, void*& end) {
    // CentralCache是所有线程共享的所以访问对应的桶要加锁
    spanLists[index].mtx.lock();

    Span* span = getASpan(index, size); 

    size_t actualNum = span->freeList.PopRange(batchNum, start, end);

    spanLists[index].mtx.unlock();

    return actualNum;
    //TODO
}