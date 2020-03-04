#include "CQueue.hpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

CQueue_L<int> lockQueue;
CQueue_LF<int> lockFreeQueue;

int64_t sleepDurations;

bool randomSleeps = false;

void randomThreadSleep() {
    auto start = std::chrono::high_resolution_clock::now();
    int randSleep = rand() % 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(randSleep));
    
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    sleepDurations += duration;
}

void producerThread(bool lock, int min, int max) {
    for(int i = min; i <= max; i++) {
        if(randomSleeps) {
            randomThreadSleep();
        }
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
                cout << val << "\n";
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
    bool lock = false;
    int producerThreads = 100;
    int consoomThreads = 1;
    int producerWork = 1;

    int c = 0;
    while((c = getopt(argc, argv, "p:c:w:ls"))!= -1) {
        switch(c) {
            case 'p':
                producerThreads = atoi(optarg);
                break;
            case 'c':
                consoomThreads = atoi(optarg);
                break;
            case 'w':
                producerWork = atoi(optarg);
                break;
            case 'l':
                lock = true;
                break;
            case 's':
                randomSleeps = true;
                break;
        }
    }

    thread queueAdd[producerThreads];
    thread queueRm[consoomThreads];

    if((producerThreads * producerWork) % consoomThreads != 0) {
        consoomThreads = 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < consoomThreads; i++) {
        queueRm[i] = thread(consoomerThread, lock, (producerThreads * producerWork) / consoomThreads);
    }

    for(int i = 0; i < producerThreads; i++) {
        queueAdd[i] = thread(producerThread, lock, i * (producerWork) + 1, (i+1) * producerWork);
    }

    for(int i = producerThreads - 1; i >= 0; i--) {
        queueAdd[i].join();
    }

    for(int i = consoomThreads - 1; i >= 0; i--) {
        queueRm[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    cout << "Took " << duration << "us\n";
    cout << "Threads slept for " << sleepDurations << "us total\n";

    return 0;
}