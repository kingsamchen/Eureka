//
// Kingsley Chen
//

#pragma once

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/basic_endpoint.hpp"

namespace rest {

class server {
public:
    explicit server(boost::asio::io_context& ioc)
        : ioc_(ioc) {}

private:
    boost::asio::io_context& ioc_;
};

} // namespace rest
