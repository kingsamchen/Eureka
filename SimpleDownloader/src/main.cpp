/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "base/at_exit.h"
#include "base/command_line.h"

#include "downloader/url_downloader.h"

class DownloadCompleteEvent : public bililive::URLDownloader::CompleteCallback {
public:
    void OnDownloadSuccess() override
    {
        std::cout << "on download success\n";
    }

    void OnDownloadStopped() override
    {
        std::cout << "on download stopped\n";
    }

    void OnDownloadFailure() override
    {
        std::cout << "on download failure\n";
    }
};

int main(int argc, char* argv[])
{
    CommandLine::Init(0, nullptr);
    base::AtExitManager exit_manager;

    base::MessageLoopForIO loop;

    GURL url("http://photo.fanfou.com/v1/mss_3d027b52ec5a4d589e68050845611e68/ff/n0/0d/ab/yr_164422.jpg@596w_1l.jpg");
    base::FilePath path(L"D:\\misc\\test.jpg");
    DownloadCompleteEvent complete_event;
    bililive::URLDownloader downloader(url, path, &complete_event);
    downloader.Start();

    loop.Run();

    return 0;
}