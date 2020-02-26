#include <stack>

using namespace std;

// 核心思路
// 使用两个栈来转移数据
// 为了提高效率，尽在必要的时候切换栈
// master栈为常规使用的栈，push数据时都会往master里压
// slave为pop操作专用的栈，当需要pop时，将所有数据转移到slave栈中，然后弹出栈顶
// 好处是对于连续的push，或者连续的pop，开销很低
class MyQueue {
    stack<int> master_;
    stack<int> slave_;
    int head_;

public:
    /** Initialize your data structure here. */
    MyQueue() {
    }

    /** Push element x to the back of queue. */
    void push(int x) {
        if (!slave_.empty()) {
            head_ = slave_.top();
            do {
                auto ele = slave_.top();
                slave_.pop();
                master_.push(ele);
            } while (!slave_.empty());
        }

        master_.push(x);
        if (master_.size() == 1) {
            head_ = x;
        }
    }

    /** Removes the element from in front of queue and returns that element. */
    int pop() {
        while (!master_.empty()) {
            auto ele = master_.top();
            master_.pop();
            slave_.push(ele);
        }

        auto result = slave_.top();
        slave_.pop();
        return result;
    }

    /** Get the front element. */
    int peek() {
        return slave_.empty() ? head_ : slave_.top();
    }

    /** Returns whether the queue is empty. */
    bool empty() {
        return master_.empty() && slave_.empty();
    }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */
