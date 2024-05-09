#pragma once
// #include "AlignNum.h"
#include "FreeList.h"
#include "SpanList.h"
#include <cstddef>


// 只能有一个实例
class CentralCache{
public:
    static inline CentralCache* getInstance() {
        return &centralCache;
    }
    size_t fetchRangeObj(size_t size, size_t batchNum, void*& start, void*& end);

private:
    CentralCache() = default;
    CentralCache(const CentralCache&) = delete;
    CentralCache(CentralCache&&) = delete;
    CentralCache& operator=(const CentralCache&) = delete;
    CentralCache& operator=(CentralCache&&) = delete;

private:
    SpanList spanLists[FREELIST_NUM];
    // 存储一个个span链表的哈希表
    static CentralCache centralCache; 
};