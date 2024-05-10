#include <iostream>
#include <thread>
#include "headers/ConcurrentAlloc.h"

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

void test2() {
    void* ptr1 = ConcurrentAlloc(5);
    void* ptr2 = ConcurrentAlloc(8);
    void* ptr3 = ConcurrentAlloc(4);
    void* ptr4 = ConcurrentAlloc(6);
    void* ptr5 = ConcurrentAlloc(3);
    cout << ptr1 << endl;
    cout << ptr2 << endl;
    cout << ptr3 << endl;
    cout << ptr4 << endl;
    cout << ptr5 << endl;

}

void test1() {
    void* ptr1 = malloc(5);
    void* ptr2 = malloc(8);
    void* ptr3 = malloc(4);
    void* ptr4 = malloc(6);
    void* ptr5 = malloc(3);
    cout << ptr1 << endl;
    cout << ptr2 << endl;
    cout << ptr3 << endl;
    cout << ptr4 << endl;
    cout << ptr5 << endl;

}

int main(int, char**){
    // test1();
    test2();

}
