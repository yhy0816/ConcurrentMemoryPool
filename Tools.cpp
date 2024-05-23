#include "headers/Tools.h"
#include "headers/FreeList.h"
#include <cstddef>
// #include <iostream>


using std::bad_alloc;


void* systemAlloc(std::size_t size) {
#ifdef _GLIBCXX_IOSTREAM 

    std::cout << "systemAlloc(" << size << ")" << std::endl;
    
#endif
#ifdef __linux__ 
    void *base_brk = sbrk(0); // 获取当前堆顶的地址
    void *new_brk = sbrk(size); // 给堆扩容size字节

    if(base_brk == (void*)-1 || new_brk == (void*)-1) {
        throw bad_alloc();
    }
    return base_brk;
#else 
    //...
#endif

}

