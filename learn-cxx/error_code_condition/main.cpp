//
// 0xCCCCCCCC
//

#include <cassert>
#include <iostream>
#include <system_error>

#include "fmt/format.h"

namespace http {

//
// General condition that will be the default condition of error.
//

enum class general_condition {
    need_redirect = 1, // any non-zero positive value is fine.
    client_side,
    server_side,
};

class http_condition_category_impl : public std::error_category {
public:
    ~http_condition_category_impl() override = default;

    [[nodiscard]] const char* name() const noexcept override {
        return "http_condition";
    }

    [[nodiscard]] std::string message(int cond) const override {
        switch (static_cast<general_condition>(cond)) {
        case general_condition::need_redirect:
            return "Need redirect request";
        case general_condition::client_side:
            return "Client side error";
        case general_condition::server_side:
            return "Server side error";
        default:
            return "Unknown http general condition";
        }
    }
};

const std::error_category& http_general_condition_category() noexcept {
    static http_condition_category_impl instance;
    return instance;
}

std::error_condition make_error_condition(general_condition e) noexcept {
    return std::error_condition(static_cast<int>(e), http_general_condition_category());
}

} // namespace http

template<>
struct std::is_error_condition_enum<http::general_condition> : std::true_type {};

namespace http {

//
// Original result/error code.
//

enum class error {
    // Redirection
    not_modified = 304,
    temporary_redirect = 307,
    permanent_rediret = 308,

    // Client error
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    request_timeout = 408,
    conflict = 409,

    // Server error
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    gateway_timeout = 504,
};

class http_error_category_impl : public std::error_category {
public:
    ~http_error_category_impl() override = default;

    [[nodiscard]] const char* name() const noexcept override {
        return "http_error";
    }

    [[nodiscard]] std::string message(int ev) const override {
        switch (static_cast<error>(ev)) {
        case error::not_modified:
            return "Not Modified";
        case error::temporary_redirect:
            return "Temporary Redirect";
        case error::permanent_rediret:
            return "Permanent Redirect";
        case error::bad_request:
            return "Bad Request";
        case error::unauthorized:
            return "Unauthorized";
        case error::forbidden:
            return "Forbidden";
        case error::not_found:
            return "Not Found";
        case error::request_timeout:
            return "Request Timeout";
        case error::conflict:
            return "Conflict";
        case error::internal_server_error:
            return "Internal Server Error";
        case error::not_implemented:
            return "Not Implemented";
        case error::bad_gateway:
            return "Bad Gateway";
        case error::service_unavailable:
            return "Service Unavailable";
        case error::gateway_timeout:
            return "Gateway Timeout";
        default:
            return "Unknown http error";
        }
    }

    [[nodiscard]]
    std::error_condition default_error_condition(int val) const noexcept override {
        switch (val / 100) {
        case 3:
            return make_error_condition(general_condition::need_redirect);
        case 4:
            return make_error_condition(general_condition::client_side);
        case 5:
            return make_error_condition(general_condition::server_side);
        default:
            // fallback to error_code self value.
            return std::error_condition(val, *this);
        }
    }
};

const std::error_category& http_error_category() noexcept {
    static http_error_category_impl instance;
    return instance;
}

std::error_code make_error_code(error e) {
    return std::error_code(static_cast<int>(e), http_error_category());
}

} // namespace http

template<>
struct std::is_error_code_enum<http::error> : std::true_type {};

namespace http {

//
// Operation condition
//

enum class operation_condition {
    should_retry = 1,
};

class http_operation_condition_category_impl : public std::error_category {
public:
    ~http_operation_condition_category_impl() override = default;

    [[nodiscard]] const char* name() const noexcept override {
        return "http_operation_condition";
    }

    [[nodiscard]] std::string message(int val) const override {
        switch (static_cast<operation_condition>(val)) {
        case operation_condition::should_retry:
            return "Should retry later";
        default:
            return "Unknown http operation condition";
        }
    }

    [[nodiscard]] bool equivalent(const std::error_code& ec, int val) const noexcept override {
        switch (static_cast<operation_condition>(val)) {
        case operation_condition::should_retry:
            return ec == error::gateway_timeout ||
                   ec == error::conflict ||
                   ec == error::request_timeout ||
                   ec == error::service_unavailable;

        default:
            return false;
        }
    }
};

const std::error_category& http_operation_condition_category() noexcept {
    static http_operation_condition_category_impl instance;
    return instance;
}

std::error_condition make_error_condition(operation_condition e) noexcept {
    return std::error_condition(static_cast<int>(e), http_operation_condition_category());
}

} // namespace http

template<>
struct std::is_error_condition_enum<http::operation_condition> : std::true_type {};

//
// use part
//

void connect_always_fail(std::string_view addr, int mock_resp_status, std::error_code& ec) {
    fmt::println("connecting to {}", addr);
    // Because `is_error_code_enum<http::error>` is true, the corresponding overload is chosed,
    // and internally will use ADL to locate the `http::make_error_code` for error_code creation.
    ec = static_cast<http::error>(mock_resp_status);
}

int main() {
    {
        std::error_code ec;
        connect_always_fail("localhost", static_cast<int>(http::error::bad_request), ec);
        assert(ec);
        assert(ec == http::error::bad_request);
        assert(ec == http::general_condition::client_side);
        assert(ec != http::operation_condition::should_retry);
    }

    {
        std::error_code ec;
        connect_always_fail("localhost", static_cast<int>(http::error::service_unavailable), ec);
        assert(ec);
        assert(ec == http::error::service_unavailable);
        assert(ec == http::general_condition::server_side);
        assert(ec == http::operation_condition::should_retry);
    }

    return 0;
}
