//
// 0xCCCCCCCC
//

#include "main/logging.hpp"

#include <string_view>

#include "gflags/gflags.h"
#include "nlohmann/json.hpp"
#include "spdlog/pattern_formatter.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

DEFINE_bool(log_to_stderr, true, "Enable logging for dev");
DEFINE_uint32(log_file_max_size, 150 * 1024 * 1024, "Maximum size in bytes per file");
DEFINE_uint32(log_file_num, 3, "Maximum number of backlog files");

namespace base {
namespace {

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

class deflate_flag final : public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg& msg, const std::tm& /*tm_time*/,
                spdlog::memory_buf_t& dest) override {
        std::string s;
        s.reserve(msg.payload.size() + 2);
        s.append(1, '{').append(msg.payload.data(), msg.payload.size()).append(1, '}');

        // Don't throw if failed, simply fallback to original message.
        nlohmann::json j = nlohmann::json::parse(s, nullptr, false);
        if (j.is_discarded()) {
            std::copy(msg.payload.begin(), msg.payload.end(), fmt::appender(dest));
            return;
        }

        const auto& content = j["msg"].get_ref<const std::string&>();
        std::copy(content.begin(), content.end(), fmt::appender(dest));

        for (const auto& [k, v] : j.items()) {
            if (k != "msg") {
                fmt::format_to(fmt::appender(dest), " {}={}", k, v.dump());
            }
        }
    }

    [[nodiscard]] std::unique_ptr<spdlog::custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<deflate_flag>();
    }
};

} // namespace

std::optional<logger> logger::pub_;

// static
void logger::init_logging(const char* argv0) {
    init_logging(argv0, [](auto prog) { return std::string{prog}; });
}

// static
void logger::init_logging(const char* argv0,
                          const std::function<std::string(std::string_view prog)>& name_gen) {
    const std::string_view prog(argv0);
    const auto pos = prog.rfind('/');
    prog_ = pos == std::string_view::npos ? prog : prog.substr(pos + 1);

    constexpr const char json_pattern[] = "{\"timestamp\":\"%Y-%m-%dT%H:%M:%S.%e%z\","
                                          "\"level\":\"%l\",\"pid\":%P,\"tid\":%"
                                          "t,\"src\":\"%@\",%v}";
    constexpr const char console_pattern[] = "%Y-%m-%dT%H:%M:%S.%e%z %l %s:%# %*";

    const std::string basename = name_gen(prog_);

    std::vector<spdlog::sink_ptr> sinks;

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            fmt::format("/tmp/{}.public.log", basename), FLAGS_log_file_max_size,
            FLAGS_log_file_num);
    file_sink->set_pattern(json_pattern);
    sinks.push_back(std::move(file_sink));

    if (FLAGS_log_to_stderr) {
        auto stderr_sink = std::make_shared<spdlog::sinks::stderr_sink_mt>();
        auto fmtter = std::make_unique<spdlog::pattern_formatter>();
        fmtter->add_flag<deflate_flag>('*').set_pattern(console_pattern);
        stderr_sink->set_formatter(std::move(fmtter));
        sinks.push_back(std::move(stderr_sink));
    }

    auto pub_impl =
            std::make_shared<spdlog::logger>(std::string{prog_}, sinks.begin(), sinks.end());
    pub_.emplace(std::move(pub_impl), passkey{});
}

} // namespace base
