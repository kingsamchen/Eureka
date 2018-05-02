/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <algorithm>
#include <thread>
#include <vector>
#include <numeric>

constexpr size_t kMaxThreadCount = 4;
constexpr size_t kMinThreadCount = 2;

template<typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init)
{
    if (begin == end) {
        return init;
    }

    auto count = std::distance(begin, end);
    auto thread_count = std::min(kMaxThreadCount,
                                 std::max<size_t>(std::thread::hardware_concurrency(), kMinThreadCount));
    std::vector<std::thread> extra_threads;
    std::vector<T> parts(thread_count);

    size_t max_quota = (count + thread_count - 1) / thread_count;
    for (size_t i = 1; i < thread_count; ++i) {
        auto new_begin = std::next(begin, max_quota * (i - 1));
        auto new_end = std::next(new_begin, max_quota);
        extra_threads.push_back(std::thread([&parts](auto b, auto e, size_t seq) {
            auto idx = seq - 1;
            parts[idx] = std::accumulate(b, e, parts[idx]);
        }, new_begin, new_end, i));
    }

    parts.back() = std::accumulate(std::next(begin, (thread_count - 1) * max_quota), end,
                                   parts.back());

    std::for_each(extra_threads.begin(), extra_threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(parts.begin(), parts.end(), init);
}
