//
// 0xCCCCCCCC
//

#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>

#include "fmt/format.h"
#include "spdlog/spdlog.h"

namespace base {

struct fmt_with_loc {
    using source_location = std::source_location;

    std::string_view fmt;
    spdlog::source_loc loc;

    template<typename String,
             std::enable_if_t<std::is_convertible_v<String, std::string_view>, int> = 0>
    constexpr explicit(false)
            fmt_with_loc(String&& fmt_str,
                         const source_location& location = source_location::current())
        : fmt{std::forward<String>(fmt_str)}, loc{to_spd_loc(location)} {}

    [[nodiscard]] static constexpr spdlog::source_loc to_spd_loc(const source_location& location) {
        return spdlog::source_loc{location.file_name(), static_cast<std::int32_t>(location.line()),
                                  location.function_name()};
    }
};

// Redacted view of an arg.
template<typename T>
struct redacted {
    const T& value; // NOLINT(*-avoid-const-or-ref-data-members)
};

template<typename Str, typename T>
struct log_field {
    const Str& key; // NOLINT(*-avoid-const-or-ref-data-members)
    const T& value; // NOLINT(*-avoid-const-or-ref-data-members)
};

template<typename... Ts, typename F, std::size_t... I>
void consume_impl(const std::tuple<Ts...>& tuple, F&& fn, std::index_sequence<I...> /*unused*/) {
    ((std::forward<F>(fn)(std::get<I>(tuple))), ...);
}

template<typename... Ts, typename F>
void consume_fields(const std::tuple<Ts...>& fields, F&& fn) {
    using idx_seq_t =
            std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<decltype(fields)>>>;
    consume_impl(fields, std::forward<F>(fn), idx_seq_t{});
}

class logger {
    struct passkey {
        explicit passkey() = default;
    };

public:
    using ContextValue = std::variant<std::string, int64_t, double>;
    using Context = std::unordered_map<std::string, ContextValue>;

    static void init_logging(const char* argv0);

    static void init_logging(const char* argv0,
                             const std::function<std::string(std::string_view prog)>& name_gen);

    [[nodiscard]] static const logger& get_pub() {
        assert(pub_.has_value());
        return *pub_;
    }

    logger(std::shared_ptr<spdlog::logger>&& impl, passkey /*unused*/)
        : spd_impl_(std::move(impl)) {}

    ~logger() = default;

    logger(const logger&) = delete;
    logger(logger&&) = delete;
    logger& operator=(const logger&) = delete;
    logger& operator=(logger&&) = delete;

    template<typename T>
    static auto redact(const T& arg) -> redacted<T> {
        return {arg};
    }

    template<typename Str, typename T>
    static auto field(const Str& key, const T& value) -> log_field<Str, T> {
        return log_field{key, value};
    }

    template<typename... Args, typename... Ts>
    void info(const std::tuple<Ts...>& fields, fmt_with_loc fwl, Args&&... args) const {
        auto buf = build_message_buf(fields, fwl.fmt, fmt::make_format_args(args...));
        spd_impl_->log(fwl.loc, spdlog::level::info, std::string_view{buf.data(), buf.size()});
    }

    // template<typename... Args>
    // void info(const Context& ctx, fmt_with_loc fwl, Args&&... args) const {
    //     auto buf = build_message_buf(ctx, fwl.fmt, fmt::make_format_args(args...));
    //     spd_impl_->log(fwl.loc, spdlog::level::info, std::string_view{buf.data(), buf.size()});
    // }

    template<typename... S, typename... Ts>
    static auto with_fields(log_field<S, Ts>... fields) -> std::tuple<log_field<S, Ts>...> {
        return std::make_tuple(fields...);
    }

private:
    template<typename... Ts>
    static spdlog::memory_buf_t build_message_buf(const std::tuple<Ts...>& fields,
                                                  std::string_view fmt, fmt::format_args args) {
        spdlog::memory_buf_t buf;
        auto out_it = fmt::appender(buf);

        consume_fields(fields, [&out_it](const auto& field) {
            out_it = fmt::format_to(out_it, R"("{}":"{}",)", field.key, field.value);
        });

        static constexpr std::string_view msg_tag = R"("msg":")";
        out_it = std::copy(msg_tag.begin(), msg_tag.end(), out_it);
        out_it = fmt::vformat_to(out_it, fmt, args);
        *out_it++ = '"';

        return buf;
    }

    std::shared_ptr<spdlog::logger> spd_impl_;

    //
    // Shared by all instances.
    //

    inline static std::string_view prog_;

    // Used for lazy initialization and avoid dynamic allocation.
    static std::optional<logger> pub_;
};

} // namespace base

template<typename T>
struct fmt::formatter<base::redacted<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const base::redacted<T>& redacted, format_context& ctx) const {
        return format_to(ctx.out(), "{{E}}{}{{/E}}", redacted.value);
    }
};
