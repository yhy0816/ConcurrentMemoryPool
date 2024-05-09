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

int main(int, char**){
    Test();

}
