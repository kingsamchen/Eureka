//
// Kingsley Chen
//

#include <iostream>

#include "nlohmann/json.hpp"

#include "serde/json_serde.h"

struct Extra {
    std::string s1;
    std::string s2;

    JSON_SERDE_DERIVE_TYPE(Extra,
                           (s1, "s_1", json_serde::with_options(json_serde::omit_empty))
                           (s2, "s_2", json_serde::with_options(json_serde::omit_empty)))
};

class Message {
public:
    Message() = default;

    Message(int seq, int ts, std::string data)
        : seq_(seq),
          ts_(ts),
          data_(std::move(data)) {}

    [[nodiscard]] int seq() const noexcept {
        return seq_;
    }

    [[nodiscard]] int ts() const noexcept {
        return ts_;
    }

    [[nodiscard]] const std::string& data() const noexcept {
        return data_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Message& m) {
        os << "seq=" << m.seq() << "\n"
           << "ts=" << m.ts() << "\n"
           << "ints.size()=" << m.ints_.size() << "\n"
           << "extra.s1=" << m.extra_.s1 << " extra.s2=" << m.extra_.s2;
        return os;
    }

    JSON_SERDE_DERIVE_TYPE(
        Message,
        (seq_, "seq")
        (ts_, "ts")
        (ints_, "ints")
        (data_, "data", json_serde::with_options(json_serde::omit_empty,
                                                 json_serde::base64_encoded))
        (extra_, "extra", json_serde::with_options(json_serde::omit_empty)))

    int seq_{0};
    int ts_{0};
    std::vector<int> ints_;
    std::string data_;
    Extra extra_;
};

int main() {
    auto j = R"(
        {
            "seq": 42,
            "ts": 10240,
            "ints": null,
            "data": null,
            "extra": null
        }
    )"_json;
    Message msg;
    j.get_to(msg);
    std::cout << msg.extra_.s1.empty();
    return 0;
}
