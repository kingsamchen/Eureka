//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERIALIZATION_CONNECTION_HPP
#define SERIALIZATION_CONNECTION_HPP

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <functional>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace s11n_example {

/// The connection class provides serialization primitives on top of a socket.
/**
 * Each message sent using this class consists of:
 * @li An 8-byte header containing the length of the serialized data in
 * hexadecimal.
 * @li The serialized data.
 */
class connection {
public:
    /// Constructor.
    connection(const boost::asio::any_io_executor& ex)
        : socket_(ex) {
    }

    /// Get the underlying socket. Used for making a connection or for accepting
    /// an incoming connection.
    boost::asio::ip::tcp::socket& socket() {
        return socket_;
    }

    /// Asynchronously write a data structure to the socket.
    template<typename T, typename Handler>
    void async_write(const T& t, Handler handler) {
        // Serialize the data first so we know how large it is.
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << t;
        outbound_data_ = archive_stream.str();

        // Format the header.
        std::ostringstream header_stream;
        header_stream << std::setw(header_length)
                      << std::hex << outbound_data_.size();
        if (!header_stream || header_stream.str().size() != header_length) {
            // Something went wrong, inform the caller.
            boost::system::error_code error(boost::asio::error::invalid_argument);
            boost::asio::post(socket_.get_executor(), std::bind(handler, error));
            return;
        }
        outbound_header_ = header_stream.str();

        // Write the serialized data to the socket. We use "gather-write" to send
        // both the header and the data in a single write operation.
        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(outbound_header_));
        buffers.push_back(boost::asio::buffer(outbound_data_));
        boost::asio::async_write(socket_, buffers, handler);
    }

    /// Asynchronously read a data structure from the socket.
    template<typename T, typename Handler>
    void async_read(T& t, Handler handler) {
        // Issue a read operation to read exactly the number of bytes in a header.
        void (connection::*f)(const boost::system::error_code&, T&, std::tuple<Handler>) = &connection::handle_read_header<T, Handler>;
        boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
                                std::bind(f,
                                          this, boost::asio::placeholders::error, std::ref(t),
                                          std::make_tuple(handler)));
    }

    /// Handle a completed read of a message header.
    template<typename T, typename Handler>
    void handle_read_header(const boost::system::error_code& e,
                            T& t, std::tuple<Handler> handler) {
        if (e) {
            std::get<0>(handler)(e);
        } else {
            // Determine the length of the serialized data.
            std::istringstream is(std::string(inbound_header_, header_length));
            std::size_t inbound_data_size = 0;
            if (!(is >> std::hex >> inbound_data_size)) {
                // Header doesn't seem to be valid. Inform the caller.
                boost::system::error_code error(boost::asio::error::invalid_argument);
                std::get<0>(handler)(error);
                return;
            }

            // Start an asynchronous call to receive the data.
            inbound_data_.resize(inbound_data_size);
            void (connection::*f)(
                    const boost::system::error_code&,
                    T&, std::tuple<Handler>) = &connection::handle_read_data<T, Handler>;
            boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
                                    std::bind(f, this,
                                              boost::asio::placeholders::error, std::ref(t), handler));
        }
    }

    /// Handle a completed read of message data.
    template<typename T, typename Handler>
    void handle_read_data(const boost::system::error_code& e,
                          T& t, std::tuple<Handler> handler) {
        if (e) {
            std::get<0>(handler)(e);
        } else {
            // Extract the data structure from the data just received.
            try {
                std::string archive_data(&inbound_data_[0], inbound_data_.size());
                std::istringstream archive_stream(archive_data);
                boost::archive::text_iarchive archive(archive_stream);
                archive >> t;
            } catch (std::exception& e) {
                // Unable to decode data.
                boost::system::error_code error(boost::asio::error::invalid_argument);
                std::get<0>(handler)(error);
                return;
            }

            // Inform caller that data has been received ok.
            std::get<0>(handler)(e);
        }
    }

private:
    /// The underlying socket.
    boost::asio::ip::tcp::socket socket_;

    /// The size of a fixed length header.
    enum { header_length = 8 };

    /// Holds an outbound header.
    std::string outbound_header_;

    /// Holds the outbound data.
    std::string outbound_data_;

    /// Holds an inbound header.
    char inbound_header_[header_length];

    /// Holds the inbound data.
    std::vector<char> inbound_data_;
};

typedef std::shared_ptr<connection> connection_ptr;

} // namespace s11n_example

#endif // SERIALIZATION_CONNECTION_HPP
