#pragma once

#include <cstddef>

void* ConcurrentAlloc(size_t size);

void ConcurrentFree(void* ptr) ;