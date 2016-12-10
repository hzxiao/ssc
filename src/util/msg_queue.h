//
// Created by hz on 16-11-8.
//

#ifndef SSCAR_MSG_QUEUE_H
#define SSCAR_MSG_QUEUE_H

#include <string>
#include <deque>
#include <mutex>
#include "sema.h"

using namespace std;

template <typename T>
class MsgQueue {
public:
    MsgQueue();
    MsgQueue(const string queueName);
    ~MsgQueue();

    T receiveMsg();
    void sendMsg(T msg);
private:
    string queueName_;
    Semaphore *sem_{nullptr};
    deque<T> queue_;
    mutex mutex_;
};

template <typename T>
MsgQueue<T>::MsgQueue() {
//    this->queueName_ = queueName;
    sem_ = new Semaphore(0);
}

template <typename T>
MsgQueue<T>::MsgQueue(const string queueName) {
    this->queueName_ = queueName;
    sem_ = new Semaphore(0);
}

template <typename T>
MsgQueue<T>::~MsgQueue() {

}

template <typename T>
T
MsgQueue<T>::receiveMsg() {
    sem_->Get();
    mutex_.lock();

    if (queue_.empty()) {
        mutex_.unlock();
        return nullptr;
    }

    T msg = queue_.front();
    queue_.pop_front();

    mutex_.unlock();
    return msg;
}

template <typename T>
void
MsgQueue<T>::sendMsg(T msg) {
    mutex_.lock();

    queue_.push_back(msg);

    mutex_.unlock();
    sem_->Post();
}

#endif //SSCAR_MSG_QUEUE_H
