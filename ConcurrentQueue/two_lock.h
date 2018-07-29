/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TWO_LOCK_H_
#define TWO_LOCK_H_

#include <mutex>

template<typename T>
class TLQueue {
    struct Node {
        T value;
        Node* next;

        Node()
            : next(nullptr)
        {}

        explicit Node(const T& value)
            : value(value), next(nullptr)
        {}
    };

public:
    // Class invariant: `head_` points to a sentinel node; and whatever the state of the queue
    // is, it as least has one node.
    TLQueue()
    {
        auto sentinel = new Node();
        head_ = sentinel;
        tail_ = sentinel;
    }

    ~TLQueue()
    {
        while (head_) {
            auto node = head_;
            head_ = head_->next;
            delete node;
        }
    }

    void Enqueue(const T& value)
    {
        auto new_tail = new Node(value);

        std::lock_guard<std::mutex> lock(tail_mtx_);
        tail_->next = new_tail;
        tail_ = new_tail;
    }

    bool Dequeue(T& value)
    {
        Node* old_head = nullptr;

        {
            std::lock_guard<std::mutex> lock(head_mtx_);

            auto new_head = head_->next;
            // Empty queue.
            if (!new_head) {
                return false;
            }

            old_head = head_;

            value = new_head->value;
            head_ = new_head;
        }

        delete old_head;

        return true;
    }

private:
    Node* head_;
    Node* tail_;
    std::mutex head_mtx_;
    std::mutex tail_mtx_;
};

#endif  // TWO_LOCK_H_
