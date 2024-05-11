#include "headers/ConcurrentAlloc.h"
#include "headers/ThreadCache.h"
#include <new>
#include <stdexcept>

void* ConcurrentAlloc(size_t size) {

    if(size == 0) throw std::bad_alloc();
    return TLSThreadCache.Allocate(size);
}

void ConcurrentFree(void* ptr, size_t size) {
    
    TLSThreadCache.Deallocate(ptr, size);
    
}