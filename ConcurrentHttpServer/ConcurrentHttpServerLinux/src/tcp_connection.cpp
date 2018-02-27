/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include <time.h>

#include <sstream>
#include <utility>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#include "kbase/base_path_provider.h"
#include "kbase/file_util.h"
#include "kbase/path_service.h"
#include "kbase/string_format.h"
#include "kbase/string_util.h"
#include "kbase/string_view.h"
#include "kbase/tokenizer.h"

#include "tcp_connection_manager.h"

namespace {

constexpr const char* kDays[] { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

constexpr const char* kMonths[] {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

std::string HttpDateFromFileTime(const kbase::FileTime& ft)
{
    auto t = kbase::FileTime::time_type::clock::to_time_t(ft.value());
    auto date = localtime(&t);

    return kbase::StringPrintf("%s, %02d %s %d %02d:%02d:%02d GMT",
                               kDays[date->tm_wday],
                               date->tm_mday,
                               kMonths[date->tm_mon],
                               date->tm_year + 1900,
                               date->tm_hour,
                               date->tm_min,
                               date->tm_sec);
}

kbase::StringView MimeTypeFromExtension(const kbase::Path::string_type& ext)
{
    constexpr std::pair<kbase::StringView, kbase::StringView> table[] {
        {kbase::StringView(""), kbase::StringView("application/octet-stream")},
        {kbase::StringView(".html"), kbase::StringView("text/html")},
        {kbase::StringView(".htm"), kbase::StringView("text/html")},
        {kbase::StringView(".txt"), kbase::StringView("text/plain")},
        {kbase::StringView(".wav"), kbase::StringView("audio/wav")},
        {kbase::StringView(".gif"), kbase::StringView("image/gif")},
        {kbase::StringView(".jpg"), kbase::StringView("image/jpeg")},
        {kbase::StringView(".png"), kbase::StringView("image/png")},
        {kbase::StringView(".bmp"), kbase::StringView("image/bmp")}
    };

    auto it = std::find_if(std::begin(table), std::end(table), [&ext](const auto& e) {
                  return e.first == ext;
              });

    return it != std::end(table) ? it->second : table[0].second;
}

bool IsRequestComplete(const std::string& req, size_t bytes_read)
{
    constexpr const char* kTerminator = "\r\n\r\n";

    auto last_size = req.size() - bytes_read;

    // We may have received \r \n \r in last read.
    size_t start_pos = last_size > 3 ? last_size - 3 : 0;

    return std::string::npos != req.find(kTerminator, start_pos);
}

std::string UnescapeURL(kbase::StringView escaped_url)
{
    std::string url;
    char hex[3] {0};
    for (auto it = escaped_url.begin(); it != escaped_url.end(); ++it) {
        if (*it != '%') {
            url.append(1, *it);
        } else {
            hex[0] = *++it;
            hex[1] = *++it;
            url.append(1, static_cast<char>(strtoul(hex, nullptr, 16)));
        }
    }

    return url;
}

}   // namespace

TcpConnection::TcpConnection(ScopedFD conn_fd, const struct sockaddr_in& addr)
    : conn_fd_(std::move(conn_fd)), port_(static_cast<int>(ntohs(addr.sin_port))), response_sent_(0)
{
    inet_ntop(AF_INET, &addr.sin_addr, ip_, kIPAddrLen);
    printf("Connected to client %s:%d\n", ip_, port_);

    data_dir_ = kbase::PathService::Get(kbase::DirExe).AppendWith("data");
}

void TcpConnection::Read()
{
    while (true) {
        auto bytes_read = recv(conn_fd_.get(), io_buf_, kIOBufSize, 0);
        if (bytes_read < 0) {
            if (errno != EAGAIN) {
                printf("Read from %s:%d failed!\n", ip_, port_);
                // TODO: reset
            }

            return;
        } else if (bytes_read == 0) {
            printf("Client %s:%d disconnected!\n", ip_, port_);
            // TODO: reset
            return;
        } else {
            printf("Read from %s:%d %ld bytes\n", ip_, port_, bytes_read);
            request_.append(io_buf_, bytes_read);
            if (IsRequestComplete(request_, bytes_read)) {
                PrepareResponse();
                return;
            }
        }
    }
}

void TcpConnection::Write()
{
    while (true) {
        auto buf_begin = response_.data() + response_sent_;
        auto try_count = response_.size() - response_sent_;
        auto bytes_written = send(conn_fd_.get(), buf_begin, try_count, 0);
        if (bytes_written < 0) {
            if (errno != EAGAIN) {
                // TODO: reset
            }

            return;
        } else if (bytes_written == 0) {
            // TODO: reset
            return;
        } else {
            response_sent_ += bytes_written;
            if (response_sent_ == response_.size()) {
                request_.clear();

                epoll_event ev {0};
                ev.events = EPOLLIN | EPOLLET;
                ev.data.ptr = this;
                epoll_ctl(TcpConnectionManager::GetInstance()->epfd(), EPOLL_CTL_MOD,
                          conn_fd_.get(), &ev);

                return;
            }
        }
    }
}

void TcpConnection::PrepareResponse()
{
    auto request_line = *kbase::Tokenizer(request_, "\r\n").begin();
    auto request_uri = *++(kbase::Tokenizer(request_line, " ").begin());
    auto unescaped_uri = UnescapeURL(request_uri);

    kbase::TrimLeadingString(unescaped_uri, "/");
    kbase::Path resource_path = data_dir_.AppendWith(unescaped_uri);

    printf("-> request for %s\n", resource_path.value().c_str());

    kbase::FileInfo file_info;
    if (!kbase::GetFileInfo(resource_path, file_info)) {
        response_ = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    } else {
        std::stringstream header;
        header << "HTTP/1.1 200 OK\r\n"
               << "Date: " << HttpDateFromFileTime(file_info.last_modified_time()) << "\r\n"
               << "Server: KHTTPD/0.1.0 (Linux)\r\n"
               << "Content-Length: " << file_info.file_size() << "\r\n"
               << "Content-Type: " << MimeTypeFromExtension(resource_path.extension()) << "\r\n"
               << "\r\n";

        response_ = header.str();
        response_.append(kbase::ReadFileToString(resource_path));
    }

    response_sent_ = 0;

    epoll_event ev {0};
    ev.events = EPOLLOUT | EPOLLET;
    ev.data.ptr = this;
    epoll_ctl(TcpConnectionManager::GetInstance()->epfd(), EPOLL_CTL_MOD,
              conn_fd_.get(), &ev);
}
