#ifndef UUID_UUID_H_
#define UUID_UUID_H_

#include <cstdint>
#include <mutex>
#include <random>

namespace uuid {

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
public:
    ~uuid() = default;

    // TODO: make default_random_generator as default parameter.
    template<typename RandomGenerator>
    static uuid make_v4(RandomGenerator& generator)
    {
        uuid uuid;
        auto ptr = reinterpret_cast<uint64_t*>(&uuid.data_);
        static_assert(alignof(uuid::data) == sizeof(*ptr));
        // TODO: auto calc
        for (int i = 0; i < 2; ++i) {
            *ptr++ = generator();
        }

        uuid.set_variant();
        uuid.set_version();

        return uuid;
    }

    std::string string() const;

private:
    uuid() = default ;

    void set_version() noexcept;

    void set_variant() noexcept;

private:
    struct data {
        // 1st 8-byte
        uint64_t field_1 : 32;
        uint64_t field_2 : 16;
        uint64_t field_3 : 16;
        // 2nd 8-byte
        uint64_t field_4 : 16;
        uint64_t field_5 : 48;
    };

    static_assert(sizeof(data) == 16);
    static_assert(alignof(data) == 8);

    data data_;
};

}   // namespace uuid

#endif  // UUID_UUID_H_
