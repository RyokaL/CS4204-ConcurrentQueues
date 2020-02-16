#include "CQueue.hpp"
#include <iostream>

using namespace std;

int main() {
    CQueue_L<int> lockQueue;
    CQueue_LF<int> lockFreeQueue;
    lockQueue.enqueue(1);
    cout << lockQueue.dequeue();
}