//
// Kingsley Chen
//

#include <stdexcept>

#include "argparse/argparse.hpp"
#include "fmt/format.h"
#include "fmt/ranges.h"

#include "hashids/hashids.h"
#include "esl/strings.h"

// ./hashids-cli [-d] [--hex] <input>
// ./hashids-cli <input>            # encode
// ./hashids-cli --hex <input>      # encode hex
// ./hashids-cli -d <input>         # decode
// ./hashids-cli -d --hex <input>   # decode hex

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("hashids-cli");
    program.add_argument("-d")
            .default_value(false)
            .implicit_value(true)
            .help("decode mode");
    program.add_argument("--hex")
            .default_value(false)
            .implicit_value(true)
            .help("data is in hex string");
    program.add_argument("data").required().help("input data");

    try {
        program.parse_args(argc, argv);

        const xx::hashids hashids;
        const bool encode = !program.get<bool>("-d");
        const bool hex_data = program.get<bool>("--hex");
        const auto& input = program.get("data");
        if (encode) {
            std::string result;
            if (hex_data) {
                result = hashids.encode_hex(input);
            } else {
                auto parts = esl::strings::split(input, ',');
                std::vector<std::uint64_t> nums;
                for (auto part : parts) {
                    nums.push_back(std::stoull(std::string{part}, nullptr));
                }
                result = hashids.encode(nums.data(), nums.size());
            }
            fmt::println("{}", result);
        } else {
            if (hex_data) {
                const auto result = hashids.decode_hex(input);
                fmt::println("{}", result);
            } else {
                auto nums = hashids.decode(input);
                fmt::println("{}", nums);
            }
        }

    } catch (std::runtime_error& ex) {
        fmt::print(stderr, "Invalid command: {}", ex.what());
        std::exit(1);
    } catch (std::invalid_argument& ex) {
        fmt::print(stderr, "Invalid input data: {}", ex.what());
        std::exit(1);
    } catch (std::exception& ex) {
        fmt::print(stderr, "Unknown error: {}", ex.what());
        std::exit(1);
    }

    return 0;
}
