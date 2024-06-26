#include "headers/SpanList.h"
#include <stdexcept>


SpanList::SpanList() {
    // 虚头节点
    head = new Span();
    // 双向循环链表，所以要相连
    head->pre = head;
    head->next = head;


}

//与管理左侧相邻页的span进行合并
Span* Span::lmerge(Span* neighbor) {

    if(neighbor->pageId + neighbor->pageNum != this->pageId)
        throw std::invalid_argument("lmerge arg error");

    this->pageId = neighbor->pageId;
    this->pageNum += neighbor->pageNum ;
    delete neighbor;
    return this;

}
//与管理右侧相邻页的span进行合并
Span* Span::rmerge(Span* neighbor) {
    if(this->pageId + this->pageNum != neighbor->pageId)
        throw std::invalid_argument("lmerge arg error");

    this->pageNum += neighbor->pageNum;
    delete neighbor;
    return this;
}


Span* Span::split(size_t k) {
    //新建一个kspan
    Span* kspan = new Span;
    kspan->pageId = this->pageId;
    kspan->pageNum = k;
    this->pageId += k;
    this->pageNum -= k;
    // std::cout << " kspan->pageNum:" << kspan->pageNum << std::endl;
    return kspan;
}

bool SpanList::Empty() {
    return head->next == head;
}

void SpanList::Insert(Span* pos, Span* node) {
    if(pos == nullptr || node == nullptr) {
        throw std::invalid_argument("arg nullptr error");
    }
    Span* pre = pos->pre;

    pre->next = node;

    pos->pre = node;

    node->pre = pre;

    node->next = pos;

}


void SpanList::PushFront(Span* node) {
    Insert(head->next, node);
}

void SpanList::Erase(Span* pos){
    if(pos == nullptr) {
        throw std::invalid_argument("arg nullptr error");
    }
    if(pos == head) {
        throw std::invalid_argument("arg val error");
    }
    pos->pre->next = pos->next;
    pos->next->pre = pos->pre;
    pos->pre = nullptr;
    pos->next = nullptr;
    
    // pos 不需要 delete 删除 而是一会要回收
    //TODO 回收逻辑
}
Span* SpanList::PopFront() {
    if(this->Empty()) return nullptr;
    Span* ret = head->next;
    this->Erase(ret);
    return ret;
}

Span* SpanList::getFirstNotNULLSpan() {
    if(this->Empty()) return nullptr;
    Span* cur = head->next; // 双向循环链表

    while(cur && cur != head) {
        if(!(cur->freeList.Empty())) {
            return cur;
        }
        cur = cur->next;
    }

    return nullptr;
}