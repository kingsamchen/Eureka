/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/threading/thread.h"

#include "downloader/url_downloader.h"

class DownloadCompleteEvent : public bililive::URLDownloader::CompleteCallback {
public:
    void OnDownloadSuccess() override
    {
        std::cout << "on download success\n";
    }

    void OnDownloadFailure() override
    {
        std::cout << "on download failure\n";
    }
};

//GURL url("http://photo.fanfou.com/v1/mss_3d027b52ec5a4d589e68050845611e68/ff/n0/0d/ab/yr_164422.jpg@596w_1l.jpg");
GURL url("https://download.microsoft.com/download/8/4/A/84A35BF1-DAFE-4AE8-82AF-AD2AE20B6B14/directx_Jun2010_redist.exe");
//base::FilePath path(L"D:\\misc\\test.jpg");
base::FilePath path(L"D:\\misc\\dx_redist.exe");
DownloadCompleteEvent complete_event;
std::unique_ptr<downloader::URLDownloader> downloader;

int main(int argc, char* argv[])
{
    CommandLine::Init(0, nullptr);
    base::AtExitManager exit_manager;

    base::Thread worker_thread("worker");
    base::Thread::Options thread_options(base::MessageLoop::TYPE_IO, 0);
    DCHECK(worker_thread.StartWithOptions(thread_options));

    using thread_task_fn = void(*)();

    // Seems URLRequestContext requires construction and starting request must be on the same thread.
    thread_task_fn run_download = []() {
        downloader = std::make_unique<downloader::URLDownloader>(url, path, &complete_event);
        downloader->Start();
        std::cout << "download started\n";
    };

    worker_thread.message_loop_proxy()->PostTask(FROM_HERE, base::Bind(run_download));

    thread_task_fn stop_download = []() {
        downloader->Stop();
        std::cout << "download stopped\n";
    };

    worker_thread.message_loop_proxy()->PostDelayedTask(FROM_HERE, base::Bind(stop_download),
                                                        base::TimeDelta::FromSeconds(30));

    std::cin.get();
    worker_thread.Stop();

    return 0;
}