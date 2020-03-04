// class CQueue_LF : CQueue<T> {
//     public:    
//         void enqueue(T payload) override {
//             Node<T>* newNode = new Node<T>(payload);
//             Node<T>* oldTail;
//             while(true) {
//                 //If our old tail was null, then update both tail and head.
//                 if(__sync_bool_compare_and_swap(&(this->tail), static_cast<Node<T>*>(nullptr), newNode)) {
//                     this->head = newNode;
//                     break;
//                 }
//                 //Get a pointer to the (now) current tail
//                 oldTail = this->tail;
//                 //Check the value we got is still valid, if not retry. If it is, set tail->next to our new node
//                 if(__sync_bool_compare_and_swap(&(this->tail->next), (oldTail->next), newNode)) {
//                     //If this fails, it is okay as it is likely another thread already updated the tail. Since the above updates the next pointer, this keeps the head
//                     //This shouldn't cause desync as the dequeue can set the tail to null which will break this check as well
//                     __sync_bool_compare_and_swap(&(this->tail), oldTail, newNode);
//                     break;
//                 }
//             }
//             //cout << "Added " << payload << " to queue\n";
//         };

//     };
//         T dequeue() override {
//             Node<T>* oldHead;
//             if(!this->head) {
//                 throw 1;
//             }
//             else {
//                 while(true) {
//                     //Get a pointer to the (now) current head
//                     oldHead = this->head;
//                     //Check the value we just got is still valid, if so set the head to the next pointer
//                     if(__sync_bool_compare_and_swap(&(this->head), oldHead, oldHead->next)) {
//                         break;
//                     }
//                 }
//                 //If the tail is the same as the old head then set to null as the head will also be null
//                 __sync_bool_compare_and_swap(&(this->tail), oldHead, static_cast<Node<T>*>(nullptr));
//                 //Return the value
//             }
//             T val = oldHead->val;
//             delete oldHead;
//             //cout << "Removed " << val << " from queue\n";
//             return val;
//         };
// };