#include <exception>
#include <iostream>

#include <vector>
#include <thread>
#include "headers/ConcurrentAlloc.h"
#include "headers/AlignNum.h"
#define DEBUG
using namespace std;

void Alloc1() {
    for(int i = 1; i <= 5; i++) 
        ConcurrentAlloc(6);
}


void Alloc2() {
    for(int i = 1; i <= 5; i++) 
        ConcurrentAlloc(7);
}

void Test() {
    std::thread t1(Alloc1);
    std::thread t2(Alloc2);
    t1.join();
    t2.join();
}




void test3() {
    void* ptr1 = ConcurrentAlloc(5);
    void* ptr2 = ConcurrentAlloc(8);
    void* ptr3 = ConcurrentAlloc(4);
    void* ptr4 = ConcurrentAlloc(6);
    void* ptr5 = ConcurrentAlloc(3);
    void* ptr6 = ConcurrentAlloc(3);
    void* ptr7 = ConcurrentAlloc(3);
    // cout << ptr1 << endl;
    // cout << ptr2 << endl;
    // cout << ptr3 << endl;
    // cout << ptr4 << endl;
    // cout << ptr5 << endl;

    ConcurrentFree(ptr1, 5);
    cout << 1 << endl;
    ConcurrentFree(ptr2, 8);
    cout << 2 << endl;
    ConcurrentFree(ptr3, 4);
    cout << 3 << endl;
    ConcurrentFree(ptr4, 6);
    cout << 4 << endl;
    ConcurrentFree(ptr5, 3);
    cout << 5 << endl;
    ConcurrentFree(ptr6, 3);
    cout << 6 << endl;
    ConcurrentFree(ptr7, 3);
    cout << 7 << endl;


}

void test1() {
    

     for(int i = 0; i < 100; i++) {
        int size = rand() % 1024 + 1;
        void* ptr = ConcurrentAlloc(size);
        cout << ptr << endl;
    }
}


void test2() {
    

   for(int i = 0; i < 100000; i++) {
        int size = rand() % 1024 + 1;
        void* ptr = malloc(size);
        // cout << ptr << endl;
    }
    
}

void MultiThreadAlloc1()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 7; ++i) // 申请7次，正好单个线程能走到pc回收cc中span的那一步
	{
		void* ptr = ConcurrentAlloc(6); // 申请的都是8B的块空间
		v.push_back(ptr);
	}

	for (auto e : v)
	{
		ConcurrentFree(e, 6);
	}
}

void MultiThreadAlloc2()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 7; ++i)
	{
		void* ptr = ConcurrentAlloc(16); // 申请的都是16B的块空间
		v.push_back(ptr);
	}

	for (int i = 0; i < 7; ++i)
	{
		ConcurrentFree(v[i], 16);
	}
}

void TestMultiThread()
{
	std::thread t1(MultiThreadAlloc1);
	std::thread t2(MultiThreadAlloc2);

	t1.join();
	t2.join();
}


int main(int, char**){

    TestMultiThread();
    
}
