#include "CQueue.hpp"
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

CQueue_L<int> lockQueue;
CQueue_LF<int> lockFreeQueue;

void randomThreadSleep() {
    int randSleep = rand() % 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(randSleep));
}

void producerThread(bool lock, int min, int max) {
    for(int i = min; i <= max; i++) {
        //randomThreadSleep();
        if(lock) {
            lockQueue.enqueue(i);
        }
        else {
            lockFreeQueue.enqueue(i);
        }
    }
}

void consoomerThread(bool lock, int loops) {
    int count = 0;
    int val = 0;
    bool removed = false;
    int itemsRemoved = 0;
    for(int i = 0; i < loops; i++) {
        removed = false;
        val = 0;
        count = 0;
        //randomThreadSleep();
        while(!removed) {
            try {
                if(lock) {
                    val = lockQueue.dequeue();
                }
                else {
                    val = lockFreeQueue.dequeue();
                }
                //cout << "Tried: " << count << " times before removing " << val << "\n";
                itemsRemoved += 1;
                removed = true;
            }
            catch (int e) {
                count += 1;
                //Nothing in queue, try again
            }
        }
    }
    cout << "Removed: " << itemsRemoved << " items\n";
}

int main(int argc, char* argv[]) {
    srand(time(0));
    bool lock = true;
    int producerThreads = 100;
    int producerWork = 1;

    if(argc > 1) {
        producerThreads = stoi(argv[1]);
    }
    if(argc > 2) {
        producerWork = stoi(argv[2]);
    }
    if(argc > 3) {
        lock = stoi(argv[3]);
    }

    thread queueAdd[100];

    auto start = std::chrono::high_resolution_clock::now();
    
    thread consoomer(consoomerThread, lock, (producerThreads * producerWork));

    for(int i = 0; i < producerThreads; i++) {
        queueAdd[i] = thread(producerThread, lock, 1, producerWork);
    }

    for(int i = 0; i < producerThreads; i++) {
        queueAdd[i].join();
    }

    consoomer.join();

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    cout << "Took " << duration << "us\n";

    return 0;
}