//
// logger_service.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERVICES_LOGGER_SERVICE_HPP
#define SERVICES_LOGGER_SERVICE_HPP

#include <boost/asio.hpp>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <thread>

namespace services {

/// Service implementation for the logger.
class logger_service
    : public boost::asio::execution_context::service {
public:
    /// The type used to identify this service in the execution context.
    typedef logger_service key_type;

    /// The backend implementation of a logger.
    struct logger_impl {
        explicit logger_impl(const std::string& ident) : identifier(ident) {}
        std::string identifier;
    };

    /// The type for an implementation of the logger.
    typedef logger_impl* impl_type;

    /// Constructor creates a thread to run a private io_context.
    logger_service(boost::asio::execution_context& context)
        : boost::asio::execution_context::service(context),
          work_io_context_(),
          work_(boost::asio::make_work_guard(work_io_context_)),
          work_thread_([this] { work_io_context_.run(); }) {
    }

    logger_service(const logger_service&) = delete;
    logger_service& operator=(const logger_service&) = delete;
    ;

    /// Destructor shuts down the private io_context.
    ~logger_service() {
        /// Indicate that we have finished with the private io_context. Its
        /// io_context::run() function will exit once all other work has completed.
        work_.reset();
        if (work_thread_.joinable())
            work_thread_.join();
    }

    /// Destroy all user-defined handler objects owned by the service.
    void shutdown() {
    }

    /// Return a null logger implementation.
    impl_type null() const {
        return 0;
    }

    /// Create a new logger implementation.
    void create(impl_type& impl, const std::string& identifier) {
        impl = new logger_impl(identifier);
    }

    /// Destroy a logger implementation.
    void destroy(impl_type& impl) {
        delete impl;
        impl = null();
    }

    /// Set the output file for the logger. The current implementation sets the
    /// output file for all logger instances, and so the impl parameter is not
    /// actually needed. It is retained here to illustrate how service functions
    /// are typically defined.
    void use_file(impl_type& /*impl*/, const std::string& file) {
        // Pass the work of opening the file to the background thread.
        boost::asio::post(work_io_context_,
                          std::bind(&logger_service::use_file_impl, this, file));
    }

    /// Log a message.
    void log(impl_type& impl, const std::string& message) {
        // Format the text to be logged.
        std::ostringstream os;
        os << impl->identifier << ": " << message;

        // Pass the work of writing to the file to the background thread.
        boost::asio::post(work_io_context_,
                          std::bind(&logger_service::log_impl, this, os.str()));
    }

private:
    /// Helper function used to open the output file from within the private
    /// io_context's thread.
    void use_file_impl(const std::string& file) {
        ofstream_.close();
        ofstream_.clear();
        ofstream_.open(file.c_str());
    }

    /// Helper function used to log a message from within the private io_context's
    /// thread.
    void log_impl(const std::string& text) {
        ofstream_ << text << std::endl;
    }

    /// Private io_context used for performing logging operations.
    boost::asio::io_context work_io_context_;

    /// Work for the private io_context to perform. If we do not give the
    /// io_context some work to do then the io_context::run() function will exit
    /// immediately.
    boost::asio::executor_work_guard<
            boost::asio::io_context::executor_type>
            work_;

    /// Thread used for running the work io_context's run loop.
    std::thread work_thread_;

    /// The file to which log messages will be written.
    std::ofstream ofstream_;
};

} // namespace services

#endif // SERVICES_LOGGER_SERVICE_HPP
