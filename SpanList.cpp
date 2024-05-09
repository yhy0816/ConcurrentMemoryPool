#include "headers/SpanList.h"
#include <stdexcept>


SpanList::SpanList() {
        // 虚头节点
        head = new Span();
        // 双向循环链表，所以要相连
        head->pre = head;
        head->next = head;
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
