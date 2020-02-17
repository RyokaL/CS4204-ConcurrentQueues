#include "CQueue.hpp"
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace std;

CQueue_L<int> lockQueue;
CQueue_LF<int> lockFreeQueue;

void randomThreadSleep() {
    int randSleep = rand() % 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(randSleep));
}

void addToQueueL(int value) {
    lockQueue.enqueue(value);
    return;
}

void dequeueL(int id) {
    randomThreadSleep();
    int val = 0;
    int count = 0;
    bool removed = false;
    while(!removed) {
        try {
            val = lockQueue.dequeue();
            cout << "Tried: " << count << " times before removing " << val << "\n";
            removed = true;
        }
        catch (int e) {
            count += 1;
            //Nothing in queue, try again
        }
    }
    return;
}

int main(int argc, char* argv[]) {
    int maxThreads = stoi(argv[1]);
    thread threads[maxThreads * 2];

    if(argc == 3) {
        srand(stoi(argv[2]));
    }

    for(int i = 0; i < maxThreads; i++) {
        threads[i] = thread(addToQueueL, i);
        threads[i + maxThreads] = thread(dequeueL, i + maxThreads);
    }

    for(int i = 0; i < maxThreads; i++) {
        threads[i].join();
        threads[i + maxThreads].join();
    }

    return 0;
}