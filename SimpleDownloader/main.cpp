/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <memory>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/message_loop/message_loop.h"
#include "base/memory/scoped_ptr.h"

#include "net/base/io_buffer.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_builder.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request_status.h"
#include "url/gurl.h"

class TestURLRequestDelegate : public net::URLRequest::Delegate {
public:
    TestURLRequestDelegate() = default;

    void OnResponseStarted(net::URLRequest* request) override
    {
        std::cout << "status: " << request->status().status() << std::endl;
        std::cout << "response code: " << request->GetResponseCode() << std::endl;
        base::MessageLoopForIO::current()->Quit();
    }

    void OnReadCompleted(net::URLRequest* request, int bytes_read) override
    {}

private:
    DISALLOW_COPY_AND_ASSIGN(TestURLRequestDelegate);
};

int main(int argc, char* argv[])
{
    CommandLine::Init(0, nullptr);
    base::AtExitManager exit_manager;

    base::MessageLoopForIO io_loop;

    TestURLRequestDelegate request_delegate;
    net::URLRequestContextBuilder builder;
    std::unique_ptr<net::URLRequestContext> context(builder.Build());

    net::URLRequest request(GURL("https://www.baidu.com"), &request_delegate, context.get());
    request.Start();
    io_loop.Run();

    return 0;
}