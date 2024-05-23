cmake_minimum_required(VERSION 3.0.0)
project(ConcurrentMemoryPool VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


add_executable(ConcurrentMemoryPool 
                CentralCache.cpp
                ConcurrentAlloc.cpp
                FreeList.cpp
                main.cpp
                PageCache.cpp
                SpanList.cpp
                ThreadCache.cpp
                Tools.cpp 
                )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
