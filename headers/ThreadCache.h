#include "FreeList.h"
#include "Tools.h"

class ThreadCache{

public:
    // 线程申请空间
    void* Allocate(std::size_t size);
    // 线程释放空间
    void Deallocate(void* obj, std::size_t size);
    // 向 CentralCache 申请空间的接口
    void* FetchFromCentralCache(std::size_t size, std::size_t alignSize);

private:

    FreeListSet<FREELIST_NUM> freeLists;

};