#include "headers/PageCache.h"
#include "headers/ThreadCache.h"
#include "headers/Tools.h"
#include <stdexcept>
#include <iostream>


PageCache PageCache::pageCache;


Span* PageCache::NewSpan(size_t k) {
    // 取出一个 k页的Span

    if( k == 0 || k >= PAGE_NUM) {
        throw std::invalid_argument("NewSpan arg range error");
    }

    // 若k号桶内有span
    if(!spanLists[k].Empty()) {
        Span* span = spanLists[k].PopFront();
        size_t pageEnd = span->pageId + span->pageNum;
        for(size_t i = span->pageId; i < pageEnd; i++) {
            this->idSpanMap[i] = span;
        }

        return span;
    }

    // 若k号桶没有span而后面的桶有
    for(int i = k + 1; i < PAGE_NUM; i++) {
        if(!spanLists[i].Empty()) { // i 号桶有 span 将它拆分 一个 k页的span 和 n - k 页的span 
            Span* nspan = spanLists[i].PopFront();
            Span* kspan = nspan->split(k);
            this->spanLists[nspan->pageNum].PushFront(nspan);
            //把 n - k 页的边缘也映射一下
            this->idSpanMap[nspan->pageId] = nspan;
            this->idSpanMap[nspan->pageId + nspan->pageNum - 1] = nspan;

            //映射到哈希表中
            size_t pageEnd = kspan->pageId + kspan->pageNum;
            for(size_t i = kspan->pageId; i < pageEnd; i++) {
                this->idSpanMap[i] = kspan;
            } 

            return kspan;
        }
    }


    //若k号桶和后面的桶都没有
    //直接向系统申请128页span

    void* ptr = systemAlloc((PAGE_NUM - 1) << PAGE_SHIFT);
    Span* newSpan = new Span;
    // std::cout << "ptr" << ptr << std::endl;
    
    newSpan->pageId = (size_t)ptr >> PAGE_SHIFT;
    newSpan->pageNum = PAGE_NUM - 1;
    

    spanLists[PAGE_NUM - 1].PushFront(newSpan); 

  
    return NewSpan(k);

}

size_t PageCache::getPageNum(size_t size, size_t index) {
     //tc 向 cc 申请 size 大小的块时单次最大申请块数
    size_t num = TLSThreadCache.getfreeLists(index).getMaxSize();

    size_t pagNum = num * size;

    pagNum >>= PAGE_SHIFT;
    if(pagNum == 0) pagNum = 1;
    return pagNum;
}

Span* PageCache::findSpanByAddr(void* obj) {
    size_t pageId = (size_t)obj >> PAGE_SHIFT;
    auto item = idSpanMap.find(pageId);
    if(item == idSpanMap.end()) {
        throw std::invalid_argument("obj not found error");
    }
    return item->second;
    
}



// 接受cc还回来的span
void PageCache::releaseSpanToPageCache(Span* span) {

#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "PageCache::releaseSpanToPageCache(" << span << ")" << std::endl;
    
#endif

    //分别向左右合并
    //左
    while(1) {
        size_t targetPageId = span->pageId - 1;
        auto item = idSpanMap.find(targetPageId);

        if(item == idSpanMap.end()){
            break; // 左侧没有了
        }

        Span* lspan = item->second;
        // 如果 lspan 在 cc中 
        if (lspan->isUsed) {
             break;
        }
        // 如果 和并 后大于 128 页
        if(span->pageNum + lspan->pageNum > PAGE_NUM - 1) {
            break;
        }
        idSpanMap.erase(lspan->pageId);
        idSpanMap.erase(lspan->pageId + lspan->pageNum - 1);
        spanLists[lspan->pageNum].Erase(lspan);
        span->lmerge(lspan);

    }


     //右
    while(1) {
        size_t targetPageId = span->pageId + span->pageNum;
        auto item = idSpanMap.find(targetPageId);

        if(item == idSpanMap.end()){
            break; // 右侧没有了
        }

       
        Span* rspan = item->second;
        // 如果 lspan 在 cc中 
        if (rspan->isUsed) {
            break;
        }
        // 如果 和并 后大于 128 页
        if(span->pageNum + rspan->pageNum > PAGE_NUM - 1) {
            break;
        }
        idSpanMap.erase(rspan->pageId);
        idSpanMap.erase(rspan->pageId + rspan->pageNum - 1);
        spanLists[rspan->pageNum].Erase(rspan);
        span->rmerge(rspan);
        
    }

    //合并完毕，push到桶中

    spanLists[span->pageNum].PushFront(span);
    span->isUsed = false;


    //映射span的边缘
    idSpanMap[span->pageId] = span;
    idSpanMap[span->pageId + span->pageNum - 1] = span;

}