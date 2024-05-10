#pragma once
#include <stdexcept>
#include "FreeList.h"
// #include "ThreadCache.h"

#define MAX_BYTES (256 * 1024)

class AlignNum {
public:
private:
    static size_t GetAlignNum(std::size_t bytes) {
        if(bytes <= 128) { // 8B 对齐
            return 8;

        } else if (bytes <= 1024){ // 16B

            return 16;

        } else if (bytes <= 8 * 1024) { // 128B

            return 128;

        } else if (bytes <= 64 * 1024) { // 1024B

            return 1024;

        } else if(bytes <= 256 * 1024) { // 8 * 1024B 对齐

            return 8 * 1024;
        } 
        throw std::invalid_argument("arg range error");
    }





    static std::size_t IndexHandle(std::size_t bytes) {
        if(bytes <= 128) { // 8B 对齐

            return 0;

        } else if (bytes <= 1024){ // 16B

            return 1;

        } else if (bytes <= 8 * 1024) { // 128B

            return 2;

        } else if (bytes <= 64 * 1024) { // 1024B

            return 3;

        } else if(bytes <= 256 * 1024) { // 8 * 1024B 对齐

            return 4;
        } 
        throw std::invalid_argument("arg range error");
    }

public:
    //计算对应的字节数应该放在哪个桶里
    static size_t Index(std::size_t bytes) {
        if(bytes > MAX_BYTES) 
            throw std::invalid_argument("arg range error");

        const int bases[5] = { 0, 16, 72, 128, 184};

        std::size_t alignNum = GetAlignNum(bytes);
        int off = (RoundUp(bytes) - 1) / alignNum ;
        return bases[IndexHandle(bytes)] + off;
        
    }
    // 计算对齐后的字节数
    static size_t RoundUp(std::size_t bytes) {
        
        std::size_t alignNum = GetAlignNum(bytes);
        if(bytes % alignNum == 0) return bytes;
        return (bytes / alignNum + 1) * alignNum;
    }
   
};