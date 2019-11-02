/*
 @ 0xCCCCCCCC
*/

#include <iomanip>
#include <iostream>
#include <sstream>

#include "bucket_limiter.h"

std::string NowTimestamp()
{
    namespace chrono = std::chrono;

    auto time_now = chrono::system_clock::now();
    auto duration_in_us = chrono::duration_cast<chrono::microseconds>(time_now.time_since_epoch());
    auto us_part = duration_in_us - chrono::duration_cast<chrono::seconds>(duration_in_us);

    tm local_time_now;
    time_t raw_time = chrono::system_clock::to_time_t(time_now);
    _localtime64_s(&local_time_now, &raw_time);
    std::ostringstream stream;
    stream << std::put_time(&local_time_now, "%Y%m%d %H:%M:%S.")
        << std::setfill('0') << std::setw(6) << us_part.count();
    return stream.str();
}

int main()
{
    Bucket bucket(20, std::chrono::seconds(1), 10);
    int bytes = 256;
    int buf = 64;
    while (bytes > 0) {
        bytes -= buf;
        std::cout << NowTimestamp() << " done 64-byte\n";
        bucket.Wait(buf);
    }
    return 0;
}
