#pragma once

#include <stdexcept>
#include "AlignNum.h"
#ifdef __linux__ 
    #include <unistd.h>
#else 
    //...
#endif

void* systemAlloc(std::size_t size);



