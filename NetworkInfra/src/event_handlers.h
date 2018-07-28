/*
 @ 0xCCCCCCCC
*/

#ifndef EVENT_HANDLERS_H_
#define EVENT_HANDLERS_H_

#include <functional>
#include <memory>

namespace network {

class Buffer;
class TCPConnection;

using TCPConnectionPtr = std::shared_ptr<TCPConnection>;

using ConnectionHandler = std::function<void(const TCPConnectionPtr&)>;
using MessageHandler = std::function<void(const TCPConnectionPtr&, Buffer&)>;
using CloseHandler = std::function<void(const TCPConnectionPtr&)>;

}   // namespace network

#endif  // EVENT_HANDLERS_H_
