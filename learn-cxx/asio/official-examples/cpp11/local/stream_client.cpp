//
// stream_client.cpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2025 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

using boost::asio::local::stream_protocol;

constexpr std::size_t max_length = 1024;

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: stream_client <file>\n";
            return 1;
        }

        boost::asio::io_context io_context;

        stream_protocol::socket s(io_context);
        s.connect(stream_protocol::endpoint(argv[1]));

        std::cout << "Enter message: ";
        char request[max_length];
        std::cin.getline(request, max_length);
        size_t request_length = std::strlen(request);
        boost::asio::write(s, boost::asio::buffer(request, request_length));

        char reply[max_length];
        size_t reply_length = boost::asio::read(s,
                                                boost::asio::buffer(reply, request_length));
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
#error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
