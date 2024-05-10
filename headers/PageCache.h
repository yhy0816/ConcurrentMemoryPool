#pragma once

#include "SpanList.h"
#include <mutex>

//pc的桶数
#define PAGE_NUM 129  
// 计算页数的位移数
#define PAGE_SHIFT 13
//span的最大页数


class PageCache {

public:
    static PageCache* getInstance() {
        return &pageCache;
    }

    Span* NewSpan(size_t k);
    static size_t getPageNum(size_t size, size_t index);

private:
    PageCache() = default;
    PageCache(const PageCache&) = delete;
    PageCache(PageCache&&) = delete;
    PageCache& operator=(const PageCache&) = delete;
    PageCache& operator=(PageCache&&) = delete;
private:

    SpanList spanLists[PAGE_NUM];
    static PageCache pageCache;

public:
    std::mutex mtx; 
};