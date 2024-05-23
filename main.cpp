#include <exception>
#include <iostream>

#include <vector>
#include <thread>
#include "headers/ConcurrentAlloc.h"
#include "headers/AlignNum.h"
#include <atomic>
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


/*这里测试的是让多线程申请ntimes*rounds次，比较malloc和刚写完的ConcurrentAlloc的效率*/

/*比较的时候分两种情况，
一种是申请ntimes*rounds次同一个块大小的空间，
一种是申请ntimes*rounds次不同的块大小的空间*/

/*下面的代码稍微过一眼就好*/


// ntimes 一轮申请和释放内存的次数
// rounds 轮次
// nwors表示创建多少个线程
void BenchmarkMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	std::atomic<size_t> malloc_costtime = 0;
	std::atomic<size_t> free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&, k]() {
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					//v.push_back(malloc(16)); // 每一次申请同一个桶中的块
					v.push_back(malloc((16 + i) % 8192 + 1));// 每一次申请不同桶中的块
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					free(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += (end1 - begin1);
				free_costtime += (end2 - begin2);
			}
			});
	}

	for (auto& t : vthread)
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次malloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, malloc_costtime.load());

	printf("%u个线程并发执行%u轮次，每轮次free %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, free_costtime.load());

	printf("%u个线程并发malloc&free %u次，总计花费：%u ms\n",
		nworks, nworks * rounds * ntimes, malloc_costtime.load() + free_costtime.load());
}


// 								单轮次申请释放次数 线程数 轮次
void BenchmarkConcurrentMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
	std::vector<std::thread> vthread(nworks);
	std::atomic<size_t> malloc_costtime = 0;
	std::atomic<size_t> free_costtime = 0;

	for (size_t k = 0; k < nworks; ++k)
	{
		vthread[k] = std::thread([&]() {
			std::vector<pair<void*, int>> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					//v.push_back(ConcurrentAlloc(16));
                    int size = (16 + i) % 8192 + 1;
					v.push_back({ConcurrentAlloc(size), size});
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					ConcurrentFree(v[i].first,v[i].second);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += (end1 - begin1);
				free_costtime += (end2 - begin2);
			}
			});
	}

	for (auto& t : vthread)
	{
		t.join();
	}

	printf("%u个线程并发执行%u轮次，每轮次concurrent alloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, malloc_costtime.load());

	printf("%u个线程并发执行%u轮次，每轮次concurrent dealloc %u次: 花费：%u ms\n",
		nworks, rounds, ntimes, free_costtime.load());

	printf("%u个线程并发concurrent alloc&dealloc %u次，总计花费：%u ms\n",
		nworks, nworks * rounds * ntimes, malloc_costtime.load() + free_costtime.load());
}

int main()
{
	size_t n = 10000;
	cout << "==========================================================" << endl;
	// 这里表示4个线程，每个线程申请10万次，总共申请40万次
	BenchmarkConcurrentMalloc(n, 4, 10); 
	cout << endl << endl;
	
	// 这里表示4个线程，每个线程申请10万次，总共申请40万次
	BenchmarkMalloc(n, 4, 10);
	cout << "=========================================================" << endl;

	return 0;
}


// int main(int, char**){

//     TestMultiThread();
    
// }
