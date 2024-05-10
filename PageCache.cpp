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
        return spanLists[k].PopFront();
    }

    // 若k号桶没有span而后面的桶有
    for(int i = k + 1; i < PAGE_NUM; i++) {
        if(!spanLists[i].Empty()) { // i 号桶有 span 将它拆分 一个 k页的span 和 n - k 页的span 
            Span* nspan = spanLists[i].PopFront();
            Span* kspan = nspan->split(k);
            this->spanLists[nspan->pageNum].PushFront(nspan);
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
    // std::cout << "pageId" <<  newSpan->pageId << std::endl;
    spanLists[PAGE_NUM - 1].PushFront(newSpan);    
    return NewSpan(k);

}

size_t PageCache::getPageNum(size_t size, size_t index) {
     //tc 向 cc 申请 size 大小的块时单次最大申请块数
    size_t num = TLSThreadCache.getfreeLists(index).get_maxSize();

    size_t pagNum = num * size;

    pagNum >>= PAGE_SHIFT;
    if(pagNum == 0) pagNum = 1;
    return pagNum;
}

