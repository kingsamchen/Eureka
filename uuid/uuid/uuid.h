#ifndef UUID_UUID_H_
#define UUID_UUID_H_

#include <array>
#include <cstdint>
#include <mutex>
#include <random>

namespace uuid {

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
