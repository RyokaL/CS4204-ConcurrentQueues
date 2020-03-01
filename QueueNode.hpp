#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
    public:
        Node<T>* next;
        T val;

        Node<T>() {this->next = nullptr;};
        Node<T>(T val) {this->val = val; this->next = nullptr;};
        Node<T>* getNext() {return next;};
        void setNext(Node<T>* newNext) {this->next = newNext;};
        T getVal() {return val;};
};
#endif