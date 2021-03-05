#ifndef UUID_UUID_H_
#define UUID_UUID_H_

#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <random>

#include "uuid/endian_utils.h"
#include "uuid/md5.h"

namespace uuid {

using node_id = std::array<uint8_t, 6>;

void fetch_node_id(node_id& id);

// Guaranteed to be thread-safe.
class default_random_generator {
public:
    ~default_random_generator() = default;

    default_random_generator(const default_random_generator&) = delete;

    default_random_generator(default_random_generator&&) = delete;

    default_random_generator& operator=(const default_random_generator&) = delete;

    default_random_generator& operator=(default_random_generator&&) = delete;

    static default_random_generator& instance();

    uint64_t operator()();

private:
    default_random_generator();

private:
    std::mutex mtx_;
    std::mt19937_64 engine_;
};

class clock_sequence {
private:
    // 100ns intervals.
    using duration = std::chrono::duration<uint64_t, std::ratio<1, 10'000'000>>;

public:
    ~clock_sequence() = default;

    clock_sequence(const clock_sequence&) = delete;

    clock_sequence(clock_sequence&&) = delete;

    clock_sequence& operator=(const clock_sequence&) = delete;

    clock_sequence& operator=(clock_sequence&&) = delete;

    static clock_sequence& instance();

    std::tuple<uint64_t, uint16_t> read();

private:
    clock_sequence();

    // Gets 100ns interval count since UUID epoch.
    static uint64_t get_timestamp_since_epoch();

private:
    std::mutex mtx_;
    uint64_t last_time_;
    uint16_t seq_;

    // Difference in 100ns intervals between UUID epoch (1582/10/15 00:00:00) and Unix
    // epoch (1970/01/01 00:00:00)
    static constexpr duration k_epoch_diff{122192928000000000};
};

class uuid {
private:
    enum class version : unsigned {
        v1 = 1,
        v2,
        v3,
        v4,
        v5
    };

public:
    ~uuid() = default;

    template<typename NodeFetcher=decltype(fetch_node_id)>
    static uuid make_v1(const NodeFetcher& fetcher=fetch_node_id)
    {
        uuid uuid{};

        auto [time, seq] = clock_sequence::instance().read();

        uuid.data_[0] |= time << 32;
        uuid.data_[0] |= (time & 0x0000'ffff'0000'0000ull) >> 16;
        uuid.data_[0] |= (time >> 48);

        uuid.data_[1] |= static_cast<uint64_t>(seq & 0xff00) << 48;
        uuid.data_[1] |= static_cast<uint64_t>(seq & 0xff) << 48;

        // Reversely copy into uuid.data_[1] 0 ~ 47 bits
        node_id id;
        fetcher(id);
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&uuid.data_[1]);
        for (auto it = id.rbegin(); it != id.rend();) {
            *ptr++ = *it++;
        }

        uuid.set_variant();
        uuid.set_version(version::v1);

        return uuid;
    }

    static uuid make_v3(const uuid& ns, std::string_view name)
    {
        auto part1 = HostToNetwork(ns.data_[0]);
        auto part2 = HostToNetwork(ns.data_[1]);

        MD5Context ctx;
        MD5Digest digest;
        MD5Init(ctx);
        MD5Update(ctx, &part1, 8);
        MD5Update(ctx, &part2, 8);
        MD5Update(ctx, name.data(), name.size());
        MD5Final(ctx, digest);

        uuid uuid{};
        memcpy(uuid.data_.data(), digest.data(), digest.size());
        uuid.data_[0] = NetworkToHost(uuid.data_[0]);
        uuid.data_[1] = NetworkToHost(uuid.data_[1]);

        uuid.set_variant();
        uuid.set_version(version::v3);

        return uuid;
    }

    template<typename RandomGenerator=default_random_generator>
    static uuid make_v4(RandomGenerator& generator=default_random_generator::instance())
    {
        uuid uuid;
        uuid.data_[0] = generator();
        uuid.data_[1] = generator();

        uuid.set_variant();
        uuid.set_version(version::v4);

        return uuid;
    }

    static uuid make_from(std::string_view src)
    {
        // omit validation

        std::vector<uint64_t> parts;

        for (size_t start = 0; start != src.npos;) {
            auto pos = src.find('-', start);
            auto cnt = pos == src.npos ? src.npos : pos - start;
            auto part = src.substr(start, cnt);
            parts.push_back(std::stoull(std::string(part), nullptr, 16));
            start = pos == src.npos ? src.npos : pos + 1;
        }

        assert(parts.size() == 5);
        uuid uuid{};

        uuid.data_[0] |= parts[0] << 32;
        uuid.data_[0] |= parts[1] << 16;
        uuid.data_[0] |= parts[2];

        uuid.data_[1] |= parts[3] << 48;
        uuid.data_[1] |= parts[4];

        return uuid;
    }

    std::string string() const;

private:
    uuid() = default ;

    void set_version(version ver) noexcept;

    void set_variant() noexcept;

private:
    // data[0] -> 8-4-4
    // data[1] -> 4-12
    using data = std::array<uint64_t, 2>;

    static_assert(sizeof(data) == 16);
    static_assert(alignof(data) == 8);

    data data_;
};

}   // namespace uuid

#endif  // UUID_UUID_H_
