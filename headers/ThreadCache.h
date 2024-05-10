#pragma once
#include "Tools.h"
#include "FreeList.h"
// ThreadCache单次申请最大字节数



class ThreadCache{

public:
    // 线程申请空间
    void* Allocate(std::size_t size);
    // 线程释放空间
    void Deallocate(void* obj, std::size_t size);
    // 向 CentralCache 申请空间的接口
    void* FetchFromCentralCache(std::size_t index, std::size_t alignSize);

    FreeList& getfreeLists(size_t index){
        return freeLists[index];
    }

private:

    FreeList freeLists[FREELIST_NUM];
    // 存储freeList的哈希表


};

//tls 的全局对象， 每个线程都可以有一个独立的全局对象
extern __thread  ThreadCache TLSThreadCache;