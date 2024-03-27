//
// Kingsley Chen
//

#pragma once

#include <tuple>

#include "serde/json_serde_options.h"

#define JSON_F_SER_OPTS(member, name, opts) \
    { json_serde::json_serialize_with_opts(j, name, m.member, opts); }

#define JSON_F_DE_OPTS(member, name, opts) \
    { json_serde::json_deserialize_with_opts(j, name, m.member, opts); }

#define JSON_F_SER(member, name) \
    { j[name] = m.member; }

#define JSON_F_DE(member, name)       \
    {                                 \
        if (j.contains(name) && !j[name].is_null()) {       \
            j[name].get_to(m.member); \
        }                             \
    }

#define JSON_SERDE_EXPAND_OP_3ARGS(fn, x, y, opts, next) \
    fn##_OPTS(x, y, opts) JSON_SERDE_EXPAND_##fn##_##next

#define JSON_SERDE_EXPAND_OP_2ARGS(fn, x, y, next) fn(x, y) JSON_SERDE_EXPAND_##fn##_##next

#define JSON_SERDE_OP_3ARGS_A(fn, x, y, opts) JSON_SERDE_EXPAND_OP_3ARGS(fn, x, y, opts, B)
#define JSON_SERDE_OP_3ARGS_B(fn, x, y, opts) JSON_SERDE_EXPAND_OP_3ARGS(fn, x, y, opts, A)

#define JSON_SERDE_OP_2ARGS_A(fn, x, y) JSON_SERDE_EXPAND_OP_2ARGS(fn, x, y, B)
#define JSON_SERDE_OP_2ARGS_B(fn, x, y) JSON_SERDE_EXPAND_OP_2ARGS(fn, x, y, A)

#define JSON_SERDE_OP_1ARGS(fn, x)
#define JSON_SERDE_OP_0ARGS(fn)

#define JSON_SERDE_CHOOSE_OP(_0, _1, _2, _3, name, ...) name

// Serialization: struct to json.

#define JSON_SERDE_EXPAND_JSON_F_SER_A(...)     \
    JSON_SERDE_CHOOSE_OP(JSON_SERDE_OP_0ARGS,   \
                         ##__VA_ARGS__,         \
                         JSON_SERDE_OP_3ARGS_A, \
                         JSON_SERDE_OP_2ARGS_A, \
                         JSON_SERDE_OP_1ARGS,   \
                         JSON_SERDE_OP_0ARGS)   \
    (JSON_F_SER, ##__VA_ARGS__)

#define JSON_SERDE_EXPAND_JSON_F_SER_B(...)     \
    JSON_SERDE_CHOOSE_OP(JSON_SERDE_OP_0ARGS,   \
                         ##__VA_ARGS__,         \
                         JSON_SERDE_OP_3ARGS_B, \
                         JSON_SERDE_OP_2ARGS_B, \
                         JSON_SERDE_OP_1ARGS,   \
                         JSON_SERDE_OP_0ARGS)   \
    (JSON_F_SER, ##__VA_ARGS__)

// Deserialization: struct from json.

#define JSON_SERDE_EXPAND_JSON_F_DE_A(...)      \
    JSON_SERDE_CHOOSE_OP(JSON_SERDE_OP_0ARGS,   \
                         ##__VA_ARGS__,         \
                         JSON_SERDE_OP_3ARGS_A, \
                         JSON_SERDE_OP_2ARGS_A, \
                         JSON_SERDE_OP_1ARGS,   \
                         JSON_SERDE_OP_0ARGS)   \
    (JSON_F_DE, ##__VA_ARGS__)

#define JSON_SERDE_EXPAND_JSON_F_DE_B(...)      \
    JSON_SERDE_CHOOSE_OP(JSON_SERDE_OP_0ARGS,   \
                         ##__VA_ARGS__,         \
                         JSON_SERDE_OP_3ARGS_B, \
                         JSON_SERDE_OP_2ARGS_B, \
                         JSON_SERDE_OP_1ARGS,   \
                         JSON_SERDE_OP_0ARGS)   \
    (JSON_F_DE, ##__VA_ARGS__)

#define JSON_SERDE_DERIVE_TYPE(type, ...)                     \
    friend void to_json(nlohmann::json& j, const type& m) {   \
        JSON_SERDE_EXPAND_JSON_F_SER_A __VA_ARGS__();         \
        if (j.is_null()) {                                    \
            j = nlohmann::json::object();                     \
        }                                                     \
    }                                                         \
                                                              \
    friend void from_json(const nlohmann::json& j, type& m) { \
        if (j.is_null() || (j.is_object() && j.empty())) {    \
            return;                                           \
        }                                                     \
        JSON_SERDE_EXPAND_JSON_F_DE_A __VA_ARGS__()           \
    }

namespace json_serde {

template<typename... Opts>
auto with_options(Opts&&... opts) -> std::tuple<Opts...> {
    return std::make_tuple(std::forward<Opts>(opts)...);
}

inline constexpr omit_empty_t omit_empty;
inline constexpr base64_encoded_t base64_encoded;

} // namespace json_serde
