#include <list>
#include <unordered_map>

using namespace std;

// 核心思路
// 用map做快速查找；用双向链表做ttl维护
// 因为每次访问缓存都要更新item的TTL，所以map要保存到链表entry的迭代器，即 <key, node-reference>
// 因为evict的时候需要从链表确定要移除的item后删除map对应项，所以链表也要保存map的key，即 (key, value)
// 还有两点一定需要注意：
// 1）因为CXX里std::list更容易获取第一个元素的迭代器，所以方便考虑新元素放头部，淘汰末尾元素
// 2）put()里如果key已经存在，一定要更新entry的位置，所以需要调用get()来尝试做调整
class LRUCache {
public:
    int size_;
    int cap_;
    // cache-key -> value
    // front is newer, back is older
    list<pair<int, int>> ttl_;
    unordered_map<int, decltype(ttl_)::iterator> cache_;

    LRUCache(int capacity)
        : size_(0), cap_(capacity)
    {}

    int get(int key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return -1;
        }

        ttl_.splice(ttl_.begin(), ttl_, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        // We may overwrite an existing key!
        if (get(key) != -1) {
            cache_[key]->second = value;
            return;
        }

        // full. evict first.
        if (size_ == cap_) {
            auto item = ttl_.back();
            ttl_.pop_back();
            --size_;
            cache_.erase(item.first);
        }

        ttl_.emplace_front(key, value);
        cache_[key] = ttl_.begin();
        ++size_;
    }
};
