#include <array>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>

struct Alphabet {
    std::array<char, 62> arr;
    constexpr Alphabet()
        : arr()
    {
        for (auto ch = 'a'; ch <= 'z'; ++ch) {
            arr[ch - 'a'] = ch;
        }

        for (auto ch = 'A'; ch <= 'Z'; ++ch) {
            arr[ch - 'A' + 26] = ch;
        }

        for (auto ch = '0'; ch <= '9'; ++ch) {
            arr[ch - '0' + 52] = ch;
        }
    }

    constexpr operator std::array<char, 62>()
    {
        return arr;
    }
};

constexpr std::array<char, 62> kAlphabet = Alphabet();

int main()
{
    std::default_random_engine engine(std::random_device{}());
    std::uniform_int_distribution<> dist(0, 61);

    std::unordered_set<std::string> keys;

    constexpr size_t kNum = 100000000;

    for (auto i = 0; i < kNum; ++i) {
        std::string key;
        key.reserve(8);
        for (auto k = 0; k < 8; ++k) {
            key.append(1, kAlphabet[dist(engine)]);
        }

        keys.insert(std::move(key));
    }

    std::cout << "keys-size=" << keys.size() << "; total=" << kNum << "\n"
              << "unique ratio=" << static_cast<double>(keys.size()) / kNum;

    return 0;
}
