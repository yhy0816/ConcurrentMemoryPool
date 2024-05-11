#include "headers/ThreadCache.h"
#include "headers/AlignNum.h"
#include "headers/CentralCache.h"
#include "headers/FreeList.h"
#include <iostream>
#include <stdexcept>
// const std::size_t ThreadCache::MAX_BYTES = 256 * 1024;

void* ThreadCache::Allocate(std::size_t bytes) {
    if(bytes > MAX_BYTES) 
            throw std::invalid_argument("arg range error");

#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "ThreadCache::Allocate(" << bytes << ")" << std::endl;

#endif

    size_t index = AlignNum::Index(bytes);
    size_t alignSize = AlignNum::RoundUp(bytes);

    if(freeLists[index].Empty()) {
        return FetchFromCentralCache(index, alignSize); 
    } 
    
    return freeLists[index].pop();    

}

size_t numMoveSize(size_t size) {
    if(size == 0){
        throw std::invalid_argument("arg range error");
    }
    int num = MAX_BYTES / size;

    if(num > 512){
        return 521;
    }

    if(num < 2) {
        return 2;
    }

    return num;
}

void* ThreadCache::FetchFromCentralCache(std::size_t index, std::size_t alignSize) {
    

    // std::cout << "size : " << size << "alignSize: " << alignSize << std::endl;
#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "ThreadCache::FetchFromCentralCache(" << index << ", " << alignSize  << ")" << std::endl;
    
#endif


    size_t batchNum = freeLists[index].getMaxSize();
    // std::cout << "batchNum " << batchNum << std::endl;
    // std::cout << "index " << index << std::endl;
    if(freeLists[index].getMaxSize() < numMoveSize(alignSize)) { // 每申请一次maxSize + 1;
        freeLists[index].getMaxSize()++;                               //  达到上限就不加了 
    }
    void* start;
    void* end;
    //从cc 申请空间
    size_t actualNum = CentralCache::getInstance()->fetchRangeObj(index, alignSize, batchNum, start, end);
    
    // start 返回，NextObj(start), end 插入 freeLists[index] 桶 中
    freeLists[index].PushRange(actualNum - 1 , FreeList::NextObj(start), end); 
    return start;
}

void ThreadCache::Deallocate(void* obj, std::size_t size) {
#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "ThreadCache::Deallocate(" << obj << " " << size << ")" << std::endl;
    
#endif    
    
    if(obj == nullptr) 
        throw std::invalid_argument("arg nullptr error");
    if(size > MAX_BYTES) 
        throw std::invalid_argument("arg range error");
    std::size_t index = AlignNum::Index(size); // 计算下标
    freeLists[index].push(obj);  // 存入自由链表中
    if (freeLists[index].getSize() >= freeLists[index].getMaxSize()) {
        GiveBackToCentralCache(index, size);
    }

}

// 将 index 桶的空间归还给 cc
void ThreadCache::GiveBackToCentralCache(size_t index, size_t size) {

#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "ThreadCache::GiveBackToCentralCache(" << index << " " <<size << ")" << std::endl;
    
#endif
    
    void *start, *end;

    freeLists[index].PopRange(freeLists[index].getMaxSize(), start, end);

    CentralCache::getInstance()->releaseListToSpan(start, index, size);
}

__thread ThreadCache TLSThreadCache = ThreadCache();