#include "headers/CentralCache.h"
#include "headers/FreeList.h"
#include "headers/PageCache.h"

#include "headers/PageCache.h"
// #include <iostream>

CentralCache CentralCache::centralCache = CentralCache(); 



//从spanLists[index] 中找到一个管理非空链表的span, 如果没有，就去下层申请
Span* CentralCache::getASpan(size_t index, size_t size) {

#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "CentralCache::getASpan(" << index << " " <<size << ")" << std::endl;

#endif
    //...
    Span* aSpan = spanLists[index].getFirstNotNULLSpan();
    if(aSpan != nullptr) {
        return aSpan;
    }
    //走到这里还没返回的话就去申请了

    spanLists[index].mtx.unlock(); // 这里先把桶锁解开因为后边要去pc申请空间

    size_t pageNum = PageCache::getPageNum(size, index);
    //执行NewSpan时要给整个pageCache加锁
    PageCache::getInstance()->mtx.lock();
    Span* newSpan = PageCache::getInstance()->NewSpan(pageNum);
    newSpan->isUsed = true;    
    PageCache::getInstance()->mtx.unlock();



    char* start  = (char*)(newSpan->pageId << PAGE_SHIFT); // 这份页空间的首地址
    char* end  = (char*)(start + (newSpan->pageNum << PAGE_SHIFT)); // 这份页空间的尾地址

    // 接下来 要把这份页空间切分成size大小的链表的形式放到freeList中

    newSpan->freeList.PushBlock(size, start, end);

    spanLists[index].mtx.lock();
    this->spanLists[index].PushFront(newSpan);
    return newSpan;
}

size_t CentralCache::fetchRangeObj(size_t index, size_t size, size_t batchNum, void*& start, void*& end) {
#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "CentralCache::fetchRangeObj(" << index << " " <<size << " " << batchNum << ")" << std::endl;
    
#endif
    // CentralCache是所有线程共享的所以访问对应的桶要加锁
    spanLists[index].mtx.lock();

    Span* span = getASpan(index, size); // 从 index 这个桶里取出一个 非空 的 span
                                        // 这份span 下的链表管理的空间都是size大小的
    size_t actualNum = span->freeList.PopRange(batchNum, start, end); //从这个span中取出batchNum个size大小的内存块
    span->usecount += actualNum;

    spanLists[index].mtx.unlock();

    return actualNum;
 
}

void CentralCache::releaseListToSpan(void* start, size_t index, size_t alignSize) {
#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "CentralCache::releaseListToSpan(" << start << " " <<index << " " << alignSize << ")" << std::endl;
    
#endif

    // 将空间连接到Span上

    spanLists[index].mtx.lock();

    while(start) {
        void* next = FreeList::NextObj(start);
        Span* span = PageCache::getInstance()->findSpanByAddr(start);
        span->freeList.push(start);
        span->usecount--;
        if(span->usecount == 0) { // 这个span的所有空间都回来了，归还给pc进行管理

            spanLists[index].Erase(span);  // cc中移除这个span
            span->freeList.clear();   // 这个span的空间全回来了，也就不再需要链表管理了

            PageCache* pc = PageCache::getInstance();
            // 归还 span 到 pc 先把cc的桶锁解开，允许别的线程操作这个锁
            spanLists[index].mtx.unlock();
            // 归还 span pc 整体加锁
            pc->mtx.lock();
            pc->releaseSpanToPageCache(span);
            pc->mtx.unlock();
            spanLists[index].mtx.lock();
        }
        start = next;
        
    }

    spanLists[index].mtx.unlock();
}