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
            if(this->tail) {
                this->tail->next = newNode;
                this->tail = newNode;
            }
            else {
                this->head = newNode;
                this->tail = newNode;
            }
            //cout << "Added " << payload << " to queue\n";
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
                returnVal = this->head->val;
                this->head = this->head->next;
                if(oldHead == this->tail) {
                    this->tail = nullptr;
                }
                delete oldHead;
            }
            //cout << "Removed " << returnVal << " from queue\n";
            qLock.unlock();
            //End of critical section

            return returnVal; 
        }
};

template <typename T>
class CQueue_LF : CQueue<T> {
    public:
        Node<T>* orgHead;

        CQueue_LF() {
            this->head = new Node<T>();
            this->head->next = this->tail;
            this->tail = this->head;
            orgHead = this->head;
        }

        ~CQueue_LF() {
            delete this->head;
        }

        void enqueue(T payload) override {
            Node<T>* newNode = new Node<T>(payload);
            Node<T>* oldTail;
            while(true) {
                oldTail = this->tail;
                Node<T>* nextNull = nullptr;
                if(__atomic_compare_exchange_n(&(oldTail->next), &(nextNull), newNode, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
                    oldTail->next = newNode;
                    break;
                }
                else {
                    __atomic_compare_exchange_n(&(this->tail), &(oldTail), true, oldTail->next, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
                }
            }
            __atomic_compare_exchange_n(&(this->tail), &(oldTail), newNode, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            //cout << "Added " << payload << " to queue\n";
        }

        T dequeue() override {
            Node<T>* oldHead;
            while(true) {
                oldHead = this->head;
                if(oldHead->next == nullptr) {
                    throw 1;
                }

                if(__atomic_compare_exchange_n(&(this->head), &(oldHead), oldHead->next, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED)) {
                    break;
                }
            }
            T val = oldHead->next->val;
            delete oldHead;
            return val;
        }
};
#endif