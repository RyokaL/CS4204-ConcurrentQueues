#ifndef CQUEUE_H
#define CQUEUE_H
#include "QueueNode.hpp"
#include <atomic>
#include <mutex>
#include <iostream>

using namespace std;

template <typename T>
class CQueue {
    protected:
        Node<T>* head = nullptr;
        Node<T>* tail = nullptr;
    public:
        virtual void enqueue(T payload) = 0;
        virtual T dequeue() = 0;
};


template <typename T>
class CQueue_L : public CQueue<T> {
    private:
        mutex qLock;
    public:
        void enqueue(T payload) override {
            Node<T>* newNode = new Node<T>(payload);

            //Critical section
            qLock.lock();
            if(this->head) {
                this->tail->setNext(newNode);
                this->tail = newNode;
            }
            else {
                this->head = newNode;
                this->tail = newNode;
            }
            cout << "Added " << payload << " to queue\n";
            qLock.unlock();
            //End of critical section
        };

        T dequeue() override {
            T returnVal;
            Node<T>* oldHead;

            //Critical section
            qLock.lock();
            if(!(this->head)) {
                //cout << "Nothing in queue\n";
                qLock.unlock();
                throw 1;
            }
            else {
                oldHead = this->head;
                returnVal = this->head->getVal();
                this->head = this->head->getNext();
                delete oldHead;
                if(this->head == this->tail) {
                    this->tail = nullptr;
                }
            }
            cout << "Removed " << returnVal << " from queue\n";
            qLock.unlock();
            //End of critical section

            return returnVal; 
        };
};

template <typename T>
class CQueue_LF : public CQueue<T> {
    void enqueue(T payload) override {
        
    };

    T dequeue() override {

    };
};
#endif