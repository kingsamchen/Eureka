/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <atomic>
#include <memory>

template<typename T>
class ConcurrentStack {
private:
    struct Node {
        std::shared_ptr<T> data;
        Node* next;

        explicit Node(const T& data)
            : data(std::make_shared<T>(data))
        {}
    };

public:
    void Push(const T& data)
    {
        Node* new_node = new Node(data);
        new_node->next = head_.load();
        while (!head_.compare_exchange_weak(new_node->next, new_node))
            ;
    }

    std::shared_ptr<T> Pop()
    {
        Node* node = head_.load();

        while (node && !head_.compare_exchange_weak(node, node->next))
            ;

        return node ? node->data : std::shared_ptr<T>();
    }

private:
    std::atomic<Node*> head_;
};