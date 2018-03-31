/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

struct Stock {
    std::string name;
    unsigned int id;

    Stock(std::string name, unsigned int id)
        : name(std::move(name)), id(id)
    {}
};

class StockFactory : public std::enable_shared_from_this<StockFactory> {
public:
    StockFactory()
        : created_(0), dead_(0)
    {}

    StockFactory(const StockFactory&) = delete;
    StockFactory& operator=(const StockFactory&) = delete;

    std::shared_ptr<Stock> GetStock(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        auto& weak_stock = stock_pool_[name];
        std::shared_ptr<Stock> stock = weak_stock.lock();
        if (!stock) {
            stock.reset(new Stock(name, std::hash<std::string>{}(name)),
                        [wf = std::weak_ptr<StockFactory>(shared_from_this())](Stock* ptr) {
                            auto factory = wf.lock();
                            if (factory) {
                                factory->IncrementDead();
                            }

                            delete ptr;
                        });
            weak_stock = stock;
            ++created_;
        }

        return stock;
    }

    size_t stock_count() const
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        return stock_pool_.size();
    }

    void IncrementDead()
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        ++dead_;
    }

    void Prune()
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        auto count = stock_pool_.size();
        double zombie_ratio = static_cast<double>(dead_ - (created_ - count)) / count;
        std::cout << "zombie-ratio: " << zombie_ratio << std::endl;

        for (auto it = stock_pool_.begin(); it != stock_pool_.end();) {
            if (it->second.expired()) {
                it = stock_pool_.erase(it);
            } else {
                ++it;
            }
        }

        std::cout << count - stock_pool_.size() << " zombie nodes are pruned!\n";

        dead_ = 0;
        created_ = stock_pool_.size();
    }

private:
    mutable std::mutex pool_mutex_;
    size_t created_;
    size_t dead_;
    std::unordered_map<std::string, std::weak_ptr<Stock>> stock_pool_;
};
