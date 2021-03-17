#include <atomic>
#include <cstdio>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>

class event_handler_list {
public:
    using handler = std::function<void()>;

    event_handler_list()
        : events_(std::make_shared<std::vector<handler>>())
    {}

    ~event_handler_list() = default;

    event_handler_list(const event_handler_list&) = delete;

    event_handler_list& operator=(const event_handler_list&) = delete;

    // Two below should be thread-safe with our cow strategy.
    // `events_` is our data to protect.
    // At any given moment, there is only one writer, but may be several readers.

    // Write to shared data.
    // If the shared data has more than one alive owner, copy and swap.
    // Now that we are the sole owner, its safe to modify.
    void add_handler(handler h)
    {
        std::shared_ptr<std::vector<handler>> backup;

        // Lock must protect all operations involving `events_`.
        {
            std::lock_guard lock(mtx_);
            if (events_.use_count() > 1) {
                backup = std::make_shared<std::vector<handler>>(*events_);
                std::swap(events_, backup);
            }

            events_->push_back(std::move(h));
        }

        // backup goes destruction without lock held.
    }

    // Read from shared data.
    // Read operations occur on a copy of shared_ptr. because writer may concurrently
    // swap out the shared data.
    void handle_event()
    {
        std::shared_ptr<std::vector<handler>> reader;

        {
            std::lock_guard lock(mtx_);
            reader = events_;
        }

        for (const auto& h : *reader) {
            h();
        }
    }

private:
    std::mutex mtx_;
    std::shared_ptr<std::vector<handler>> events_;
};

int main()
{
    using namespace std::chrono_literals;

    std::atomic<bool> stop{false};

    event_handler_list ev_list;

    auto writer = [&] {
        int i = 0;
        while (stop.load() != true) {
            ev_list.add_handler([i] {
                printf("handler %d\n", i);
            });
            ++i;
            std::this_thread::sleep_for(1s);
        }
    };

    auto reader = [&] {
        while (stop.load() != true) {
            ev_list.handle_event();
            std::this_thread::sleep_for(100ms);
        }
    };

    std::thread w(writer);
    std::thread r_a(reader);
    std::thread r_b(reader);

    std::getchar();
    stop.store(true);

    w.join();
    r_a.join();
    r_b.join();

    return 0;
}
