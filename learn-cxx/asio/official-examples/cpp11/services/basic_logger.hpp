//
// basic_logger.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_BASIC_LOGGER_HPP
#define SERVICES_BASIC_LOGGER_HPP

#include <boost/asio.hpp>
#include <string>

namespace services {

/// Class to provide simple logging functionality. Use the services::logger
/// typedef.
template<typename Service>
class basic_logger {
public:
    /// The type of the service that will be used to provide timer operations.
    typedef Service service_type;

    /// The native implementation type of the timer.
    typedef typename service_type::impl_type impl_type;

    /// Constructor.
    /**
     * This constructor creates a logger.
     *
     * @param context The execution context used to locate the logger service.
     *
     * @param identifier An identifier for this logger.
     */
    explicit basic_logger(boost::asio::execution_context& context,
                          const std::string& identifier)
        : service_(boost::asio::use_service<Service>(context)),
          impl_(service_.null()) {
        service_.create(impl_, identifier);
    }

    basic_logger(const basic_logger&) = delete;
    basic_logger& operator=(basic_logger&) = delete;

    /// Destructor.
    ~basic_logger() {
        service_.destroy(impl_);
    }

    /// Set the output file for all logger instances.
    void use_file(const std::string& file) {
        service_.use_file(impl_, file);
    }

    /// Log a message.
    void log(const std::string& message) {
        service_.log(impl_, message);
    }

private:
    /// The backend service implementation.
    service_type& service_;

    /// The underlying native implementation.
    impl_type impl_;
};

} // namespace services

#endif // SERVICES_BASIC_LOGGER_HPP
