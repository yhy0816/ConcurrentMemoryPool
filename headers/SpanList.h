#pragma once
#include "FreeList.h"
#include <cstddef>
#include <mutex>



struct Span {

    size_t pageId = 0;
    size_t pageNum = 0;
    Span* pre = nullptr;
    Span* next = nullptr;
    FreeList freeList;
    size_t usecount = 0;

//将Span拆分成k 和 pageNum - k 个页的span
//当前Span变成 pageNum - k 的span 返回pageNum 为 k 的 span 指针
    Span* split(size_t k);

};


//span相连的双向循环链表
class SpanList{

public: 
    SpanList();
    void Insert(Span* pos, Span* node);
    void PushFront(Span* node);
    Span* PopFront();
    void Erase(Span* pos);
    bool Empty();
    Span* getFirstNotNULLSpan();
private:
    Span* head;
public:
    std::mutex mtx;
};