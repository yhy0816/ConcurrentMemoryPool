#pragma once
#include <stdexcept>

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

    // 求size对应在哈希表中的下标
    static inline size_t _Index(size_t size, size_t align_num)
    {							/*这里align_shift是指对齐数的二进制位数。比如size为2的时候对齐数
                                为8，8就是2^3，所以此时align_shift就是3*/
        return (size + align_num - 1) / align_num - 1;
        //这里_Index计算的是当前size所在区域的第几个下标，所以Index的返回值需要加上前面所有区域的哈希桶的个数
    }

public:
    

    // 计算映射的哪一个自由链表桶
    static inline size_t Index(size_t size)
    {

        // 每个区间有多少个链
        const int group_array[4] = { 16, 72, 128, 184 };
        if (size <= 128)
        { // [1,128] 
            return _Index(size, 8); // 3是指对齐数的二进制位位数，这里8B就是2^3B，所以就是3
        }
        else if (size <= 1024)
        { // [128+1,1024] 
            return _Index(size - 128, 16) + group_array[0];
        }
        else if (size <= 8 * 1024)
        { // [1024+1,8*1024] 
            return _Index(size - 1024, 128) + group_array[1];
        }
        else if (size <= 64 * 1024)
        { // [8*1024+1,64*1024] 
            return _Index(size - 8 * 1024, 1024) + group_array[2]
                + group_array[0];
        }
        else if (size <= 256 * 1024)
        { // [64*1024+1,256*1024] 
            return _Index(size - 64 * 1024, 8 * 1024) + group_array[3] ;
        }
        throw  std::invalid_argument("arg range error");

    }

    // 计算对齐后的字节数
    static size_t RoundUp(std::size_t bytes) {
        
        std::size_t alignNum = GetAlignNum(bytes);
        if(bytes % alignNum == 0) return bytes;
        return (bytes / alignNum + 1) * alignNum;
    }
   
};