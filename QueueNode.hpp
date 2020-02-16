#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
    private:
        Node<T>* next;
        T val;
    public:
        Node<T>(T val) {this->val = val;};
        Node<T>* getNext() {return next;};
        void setNext(Node<T>* newNext) {this->next = newNext;};
        T getVal() {return val;};
};
#endif