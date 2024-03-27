//
// Kingsley Chen
//

#pragma once

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <tuple>
#include <type_traits>

#include "cppcodec/base64_default_rfc4648.hpp"
#include "nlohmann/json.hpp"

namespace json_serde {

template<typename... T, typename F, std::size_t... I>
bool apply_serde_opts_impl(const std::tuple<T...>& tuple, F&& fn,
                           std::index_sequence<I...> /*unused*/) {
    return ((std::forward<F>(fn)(std::get<I>(tuple))) && ...);
}

template<typename... T, typename F>
bool apply_serde_opts(const std::tuple<T...>& tuple, F&& fn) {
    using idx_seq_t = std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<decltype(tuple)>>>;
    return apply_serde_opts_impl(tuple, std::forward<F>(fn), idx_seq_t{});
}

template<typename T, typename... Opt>
void json_serialize_with_opts(nlohmann::json& j,
                              std::string_view name,
                              const T& member_value,
                              const std::tuple<Opt...>& opts) {
    auto value = member_value;
    auto result = apply_serde_opts(opts, [&value](auto opt) {
        return apply_serializer_opt(value, opt);
    });

    if (result) {
        j[name] = nlohmann::json(std::move(value));
    }
}

template<typename T, typename... Opt>
void json_deserialize_with_opts(const nlohmann::json& j,
                                std::string_view name,
                                T& member_value,
                                const std::tuple<Opt...>& opts) {
    if (!j.contains(name) || j[name].is_null()) {
        return;
    }

    T value;
    j[name].get_to(value);
    auto result = apply_serde_opts(opts, [&value](auto opt) {
        return apply_deserializer_opt(value, opt);
    });

    if (result) {
        member_value = std::move(value);
    }
}

//
// options
//

struct omit_empty_t {};

template<typename T>
bool apply_serializer_opt(const T& value, omit_empty_t /*tag*/) {
    // TODO(KC): Optimize for built-in containers.
    using value_type = std::remove_cv_t<T>;
    if constexpr (std::is_integral_v<value_type>) {
        return true;
    } else if constexpr (std::is_same_v<value_type, std::string>) {
        return !value.empty();
    } else {
        auto field = nlohmann::json(value);
        return !field.empty();
    }
}

template<typename T>
bool apply_deserializer_opt(const T& /*value*/, omit_empty_t /*tag*/) {
    return true;
}

struct base64_encoded_t {};

inline bool apply_serializer_opt(std::string& value, base64_encoded_t /*tag*/) {
    value = base64::encode(value);
    return true;
}

inline bool apply_deserializer_opt(std::string& value, base64_encoded_t /*tag*/) {
    value = base64::decode<std::string>(value);
    return true;
}

} // namespace json_serde
