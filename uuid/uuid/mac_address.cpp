#include "uuid/uuid.h"

#include <cstring>
#include <memory>

#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")
#elif defined(__linux__)
#include <ifaddrs.h>
#include <linux/if_packet.h>
#include <net/if.h>
#endif

namespace {

#if defined(_WIN32) || defined(_WIN64)

bool load_adapter_address(uuid::node_id& id)
{
    // Initial buffer size in 16-KB should be enough for most cases, since it's quite
    // expensive to call `GetAdaptersAddresses()`.
    // Also, don't zero out the buffer to save a few cpu cycles.
    ULONG buf_len = 16 * 1024;
    std::unique_ptr<uint8_t[]> buf(new uint8_t[buf_len]);

    auto as_adapter_address = [](const std::unique_ptr<uint8_t[]>& raw) {
        return reinterpret_cast<IP_ADAPTER_ADDRESSES*>(raw.get());
    };

    auto rv = GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, as_adapter_address(buf), &buf_len);
    if (rv != ERROR_SUCCESS) {
        if (rv != ERROR_BUFFER_OVERFLOW) {
            return false;
        }

        // Enlarge and try again.
        buf.reset(new uint8_t[buf_len]);
        rv = GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, as_adapter_address(buf), &buf_len);
        if (rv != ERROR_SUCCESS) {
            return false;
        }
    }

    for (auto addr = as_adapter_address(buf); addr; addr = addr->Next) {
        if (addr->PhysicalAddressLength >= id.size()) {
            std::memcpy(id.data(), addr->PhysicalAddress, id.size());
            return true;
        }
    }

    return false;
}

#elif defined(__linux__)

bool load_adapter_address(uuid::node_id& id)
{
    ifaddrs* addrs = nullptr;
    if (getifaddrs(&addrs) == -1) {
        return false;
    }

    for (auto addr = addrs; addr; addr = addr->ifa_next) {
        // Also skip the loopback adapter.
        if (!addr->ifa_addr ||
            addr->ifa_addr->sa_family != AF_PACKET ||
            (addr->ifa_flags & IFF_LOOPBACK)) {
            continue;
        }

        auto phy_sock = reinterpret_cast<sockaddr_ll*>(addr->ifa_addr);
        if (phy_sock->sll_halen >= id.size()) {
            std::memcpy(id.data(), phy_sock->sll_addr, id.size());
            return true;
        }
    }

    freeifaddrs(addrs);

    return false;
}

#endif

}   // namespace

namespace uuid {

class node_id_reader {
public:
    ~node_id_reader() = default;

    node_id_reader(const node_id_reader&) = delete;

    node_id_reader(node_id_reader&&) = delete;

    node_id_reader& operator=(const node_id_reader&) = delete;

    node_id_reader& operator=(node_id_reader&&) = delete;

    static node_id_reader& instance()
    {
        static node_id_reader instance;
        return instance;
    }

    void read(node_id& id)
    {
        std::memcpy(id.data(), id_.data(), id_.size());
    }

private:
    node_id_reader()
    {
        if (load_adapter_address(id_)) {
            return;
        }

        auto& rand = default_random_generator::instance();
        auto value = rand();
        static_assert(sizeof(value) >= sizeof(id_));
        std::memcpy(id_.data(), &value, id_.size());
        // Recommended by RFC.
        id_[0] |= 0x01;
    }

private:
    node_id id_;
};


void fetch_node_id(node_id& id)
{
    node_id_reader::instance().read(id);
}

}   // namespace uuid
