#include "ThreadCache.h"
#include <iostream>

void* ConcurrentAlloc(size_t size);

void ConcurrentFree(void* ptr) ;