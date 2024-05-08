#include <iostream>
#include "headers/Tools.h"

using namespace std;

int main(int, char**){
    int tmp;
    tmp = rand() % 128 + 1;
    cout << tmp << " ";
    cout << Index(tmp) << endl;
    tmp = rand() % 1024 + 129;
    cout << tmp << " ";
    cout << Index(tmp) << endl;
    tmp = rand() % (8 * 1024) + 1025;
    cout << tmp << " ";
    cout << Index(tmp) << endl;
    tmp = rand() % (64 * 1024) + 8 * 1024 + 1;
    cout << tmp << " ";
    cout << Index(tmp) << endl;
    tmp = rand() % (256 * 1024) + 64 * 1024 + 1;
    cout << tmp << " ";
    cout << Index(tmp) << endl;
}
