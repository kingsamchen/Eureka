/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef COMMON_EVENT_HANDLERS_H_
#define COMMON_EVENT_HANDLERS_H_

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

#endif  // COMMON_EVENT_HANDLERS_H_
