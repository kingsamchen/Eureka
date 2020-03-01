#include <queue>

using namespace std;

// 核心思路
// 两个队列倒腾，保证每个操作结束后只有一个队列有数据，并且 top_ 维护栈顶数据
// push时往有数据的队列存数据，同时更新 top_
// pop时首先备份原来的top_，然后把有数据的队列的元素逐一出队（直到队尾元素），并存储另外一个队列，同时更新top_
// 弹出剩下的对位元素，那个就是要弹出的
class MyStack {
public:
    queue<int> q0_;
    queue<int> q1_;
    int top_;
    /** Initialize your data structure here. */
    MyStack() {

    }

    /** Push element x onto stack. */
    void push(int x) {
        queue<int>* master_;
        queue<int>* slave_;
        if (q0_.empty()) {
            master_ = &q1_;
        } else {
            master_ = &q0_;
        }

        master_->push(x);
        top_ = x;
    }

    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        auto ret = top_;

        queue<int>* master_;
        queue<int>* slave_;
        if (q0_.empty()) {
            master_ = &q1_;
            slave_ = &q0_;
        } else {
            master_ = &q0_;
            slave_ = &q1_;
        }

        while (master_->size() > 1) {
            auto x = master_->front();
            master_->pop();
            slave_->push(x);
            top_ = x;
        }

        master_->pop();

        return ret;
    }

    /** Get the top element. */
    int top() {
        return top_;
    }

    /** Returns whether the stack is empty. */
    bool empty() {
        return q0_.empty() && q1_.empty();
    }
};
