/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection.h"

#include <WS2tcpip.h>

#include "kbase/base_path_provider.h"
#include "kbase/basic_types.h"
#include "kbase/error_exception_util.h"
#include "kbase/logging.h"
#include "kbase/path_service.h"
#include "kbase/string_format.h"
#include "kbase/string_util.h"
#include "kbase/tokenizer.h"

#include "tcp_connection_manager.h"
#include "winsock_ctx.h"

namespace {

constexpr const char* kDays[] { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

constexpr const char* kMonths[] {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

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

uint64_t GetFileSizeFromInfo(const BY_HANDLE_FILE_INFORMATION& info)
{
    ULARGE_INTEGER ularge {0};

    ularge.LowPart = info.nFileSizeLow;
    ularge.HighPart = info.nFileSizeHigh;

    return ularge.QuadPart;
}

std::string HttpDateFromFileTime(const FILETIME& ft)
{
    SYSTEMTIME date {0};
    FileTimeToSystemTime(&ft, &date);

    return kbase::StringPrintf("%s, %02d %s %d %02d:%02d:%02d GMT",
                               kDays[date.wDayOfWeek],
                               date.wDay,
                               kMonths[date.wMonth],
                               date.wYear,
                               date.wHour,
                               date.wMinute,
                               date.wSecond);
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

    std::string ext_literal = kbase::WideToASCII(ext);

    auto it = std::find_if(std::begin(table), std::end(table), [&ext_literal](const auto& e) {
                  return e.first == ext_literal;
              });

    return it != std::end(table) ? it->second : table[0].second;
}

}   // namespace

TcpConnection::TcpConnection()
    : state_(State::WaitConnect), sent_size_(0), left_sent_size_(0), remaining_data_size_(0)
{
    Internal = 0;
    InternalHigh = 0;
    Offset = 0;
    OffsetHigh = 0;
    hEvent = nullptr;

    data_dir_ = kbase::PathService::Get(kbase::DirExe).AppendWith(L"data");
}

void TcpConnection::Conncect(ScopedSocketHandle&& conn_socket)
{
    ENSURE(CHECK, !conn_socket_).Require("Connection socket must be empty!");

    conn_socket_ = std::move(conn_socket);

    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    int len = sizeof(client_addr);
    getpeername(conn_socket_.get(), reinterpret_cast<sockaddr*>(&client_addr), &len);

    inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip_, kClientIPSize);

    printf("Connects to %s on worker %u\n", client_ip_, GetCurrentThreadId());

    ReadRequest();
}

void TcpConnection::ReadRequest()
{
    state_ = State::WaitRequest;

    buf_info_.buf = io_buf_;
    buf_info_.len = kIOBufSize;
    DWORD flags = 0;

    auto rv = WSARecv(conn_socket_.get(), &buf_info_, 1, nullptr, &flags, this, nullptr);
    if (rv == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
        LOG(WARNING) << "WSARecv() failed: " << WSAGetLastError();
        Disconnect();
    }
}

void TcpConnection::WriteResponse()
{
    state_ = State::WaitResponse;

    auto request_line = *kbase::Tokenizer(request_, "\r\n").begin();
    auto request_uri = *++(kbase::Tokenizer(request_line, " ").begin());
    auto unescaped_uri = UnescapeURL(request_uri);

    auto resource_id = kbase::UTF8ToWide(unescaped_uri);
    kbase::TrimLeadingString(resource_id, L"/");
    kbase::Path resource_path = data_dir_.AppendWith(resource_id);

    file_.reset(CreateFileW(resource_path.value().c_str(),
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            nullptr,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                            nullptr));
    if (!file_) {
        constexpr kbase::StringView header =
            "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";

        strncpy_s(io_buf_, header.data(), header.size());

        remaining_data_size_ = 0;
        left_sent_size_ = header.size();
        sent_size_ = 0;
    } else {
        BY_HANDLE_FILE_INFORMATION file_info;
        GetFileInformationByHandle(file_.get(), &file_info);

        uint64_t file_size = GetFileSizeFromInfo(file_info);

        std::stringstream header;
        header << "HTTP/1.1 200 OK\r\n"
               << "Date: " << HttpDateFromFileTime(file_info.ftLastWriteTime) << "\r\n"
               << "Server: KHTTPD/0.1.0 (Windows)\r\n"
               << "Content-Length: " << file_size << "\r\n"
               << "Content-Type: " << MimeTypeFromExtension(resource_path.extension()) << "\r\n"
               << "\r\n";

        header.read(io_buf_, kIOBufSize);
        size_t header_size = static_cast<size_t>(header.gcount());
        // We are best to make room for first part of the file in io buf.
        ENSURE(CHECK, header_size < kIOBufSize)(header_size).Require();

        auto extra = static_cast<DWORD>(kIOBufSize - header_size);
        DWORD bytes_read = 0;
        ReadFile(file_.get(), io_buf_ + header_size, extra, &bytes_read, nullptr);

        remaining_data_size_ = file_size - bytes_read;
        left_sent_size_ = header_size + bytes_read;
        sent_size_ = 0;
    }

    TransmitData();
}

void TcpConnection::TransmitData()
{
    buf_info_.buf = io_buf_ + sent_size_;
    buf_info_.len = static_cast<ULONG>(left_sent_size_);

    auto rv = WSASend(conn_socket_.get(), &buf_info_, 1, nullptr, 0, this, nullptr);
    if (rv == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
        LOG(WARNING) << "WSASend() failed: " << WSAGetLastError();
        Disconnect();
    }
}

void TcpConnection::Disconnect()
{
    state_ = State::WaitReset;
    winsock_ctx::DisconnectEx(conn_socket_.get(), this, 0, 0);
}

void TcpConnection::OnIOComplete(int64_t bytes_transferred)
{
    switch (state_) {
        case State::WaitRequest:
            OnReadComplete(bytes_transferred);
            break;

        case State::WaitResponse:
            OnWriteComplete(bytes_transferred);
            break;

        case State::WaitReset:
            OnDisconnectComplete();
            break;

        default:
            ENSURE(CHECK, kbase::NotReached())(kbase::enum_cast(state_)).Require();
            break;
    }
}

void TcpConnection::OnReadComplete(int64_t bytes_read)
{
    if (bytes_read == 0) {
        Disconnect();
        return;
    }

    request_.append(io_buf_, bytes_read);

    if (IsRequestComplete(request_, bytes_read)) {
        WriteResponse();
    } else {
        ReadRequest();
    }
}

void TcpConnection::OnWriteComplete(int64_t bytes_written)
{
    if (bytes_written == 0) {
        Disconnect();
        return;
    }

    sent_size_ += bytes_written;
    left_sent_size_ -= bytes_written;

    if (left_sent_size_ == 0) {
        if (remaining_data_size_ == 0) {
            sent_size_ = 0;
            file_ = nullptr;
            request_.clear();
            ReadRequest();
            return;
        }

        DWORD bytes_read = 0;
        ReadFile(file_.get(), io_buf_, kIOBufSize, &bytes_read, nullptr);

        remaining_data_size_ -= bytes_read;
        left_sent_size_ = bytes_read;
        sent_size_ = 0;
    }

    TransmitData();
}

void TcpConnection::OnDisconnectComplete()
{
    printf("Client %s disconnected on worker %u!\n", client_ip_, GetCurrentThreadId());

    conn_socket_.reset();

    file_ = nullptr;

    request_.clear();
    sent_size_ = 0;
    left_sent_size_ = 0;
    remaining_data_size_ = 0;

    state_ = State::WaitConnect;

    TcpConnectionManager::GetInstance()->Reclaim(this);
}
