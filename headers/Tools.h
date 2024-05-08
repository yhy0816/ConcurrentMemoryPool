#pragma once

#include <stdexcept>

#ifdef __linux__ 
    #include <unistd.h>
#else 
    //...
#endif

void* systemAlloc(std::size_t size);

size_t RoundUp(std::size_t bytes);

size_t Index(std::size_t bytes);

