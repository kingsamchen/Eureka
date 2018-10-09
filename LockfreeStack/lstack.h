
#include <atomic>
#include <cstdint>
#include <utility>

template<typename T>
class LStack {
private:
    struct Node {
        T value;
        Node* next;
    };

    struct Head {
        uintptr_t aba_tag;
        Node* node;
    };

    using AtomicHead = std::atomic<Head>;

public:
    explicit LStack(std::size_t maxsize)
        : node_buf_(static_cast<Node*>(::operator new(maxsize * sizeof(Node)))),
          head_({0, nullptr}),
          size_(0)
    {
        for (std::size_t i = 0; i < maxsize - 1; ++i) {
            node_buf_[i].next = node_buf_ + i + 1;
        }

        node_buf_[maxsize-1].next = nullptr;

        Head free_list {0, node_buf_};
        std::atomic_init(&free_, free_list);
    }

    ~LStack()
    {
        ::operator delete(node_buf_);
    }

    bool Push(const T& ele)
    {
        auto new_node = PopHead(free_);
        if (!new_node) {
            return false;
        }

        new_node->value = ele;
        PushHead(head_, new_node);

        size_.fetch_add(1);

        return true;
    }

    std::pair<bool, T> Pop()
    {
        auto ele = PopHead(head_);
        if (!ele) {
            return {false, T()};
        }

        size_.fetch_sub(1);

        std::pair<bool, T> value {true, ele->value};

        PushHead(free_, ele);

        return value;
    }

    std::size_t size() const noexcept
    {
        return size_.load();
    }

private:
    static Node* PopHead(AtomicHead& head) noexcept
    {
        Head old_head = head.load();
        Head new_head;

        do {
            if (!old_head.node) {
                return nullptr;
            }

            new_head.node = old_head.node->next;
            new_head.aba_tag = old_head.aba_tag + 1;
        } while (!head.compare_exchange_weak(old_head, new_head));

        return old_head.node;
    }

    static void PushHead(AtomicHead& head, Node* node) noexcept
    {
        Head old_head = head.load();
        Head new_head;

        do {
            node->next = old_head.node;
            new_head.node = node;
            new_head.aba_tag = old_head.aba_tag + 1;
        } while (!head.compare_exchange_weak(old_head, new_head));
    }

private:
    Node* node_buf_;
    
    AtomicHead head_;
    AtomicHead free_;

    std::atomic<std::size_t> size_;
};


