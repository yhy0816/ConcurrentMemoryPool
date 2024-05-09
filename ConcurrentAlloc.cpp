#include "headers/ConcurrentAlloc.h"
#include "headers/ThreadCache.h"

void* ConcurrentAlloc(size_t size) {
    std::cout << "ConcurrentAlloc" << std::endl;
    return TLSThreadCache.Allocate(size);
}

void ConcurrentFree(void* ptr, size_t size) {
    std::cout << "ConcurrentFree" << std::endl;
    TLSThreadCache.Deallocate(ptr, size);
    
}