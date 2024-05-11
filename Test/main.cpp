#include <exception>
#include <iostream>
#include <stdexcept>
#include <thread>
#include "headers/ConcurrentAlloc.h"
#include "headers/AlignNum.h"
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
    cout << ptr1 << endl;
    cout << ptr2 << endl;
    cout << ptr3 << endl;
    cout << ptr4 << endl;
    cout << ptr5 << endl;

}

void test1() {
    

     for(int i = 0; i < 100000; i++) {
        int size = rand() % 1024 + 1;
        void* ptr = ConcurrentAlloc(size);
        // cout << ptr << endl;
    }
}


void test2() {
    

   for(int i = 0; i < 100000; i++) {
        int size = rand() % 1024 + 1;
        void* ptr = malloc(size);
        // cout << ptr << endl;
    }
    
}

int main(int, char**){

    try{
        using std::chrono::duration_cast, std::chrono::system_clock;
        using std::chrono::milliseconds, std::chrono::seconds;
        auto millisec1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        thread t[100];
        for(int i = 1; i < 10; i++){
            t[i] = thread(test2);
            cout << "-------------" << endl;
        }
        for(int i = 1; i < 10; i++) {
            t[i].join();
        }

        auto millisec2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();    
    
        cout << millisec2 - millisec1 << endl;
    
        millisec1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        // thread t[10];
        for(int i = 1; i < 10; i++){
            t[i] = thread(test1);
            cout << "-------------" << endl;
        }
        for(int i = 1; i < 10; i++) {
            t[i].join();
        }
        millisec2 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();    

        cout << millisec2 - millisec1 << endl;

        cout << "-------------" << endl;
    } catch (std::invalid_argument e){
        cout << e.what() << endl;

    } catch (std::exception e) {
        cout << e.what() << endl;
    }
    
}
