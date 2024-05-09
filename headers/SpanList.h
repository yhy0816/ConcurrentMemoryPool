#pragma once
#include "FreeList.h"
#include <cstddef>
#include <mutex>

struct Span {

    size_t pageId = 0;
    size_t n = 0;
    Span* pre = nullptr;
    Span* next = nullptr;
    FreeList freeList;
    size_t usecount = 0;


};


//span相连的双向循环链表
class SpanList{

public: 
    SpanList();
    void Insert(Span* pos, Span* node);
    void Erase(Span* pos);

private:
    Span* head;
public:
    std::mutex mtx;
};