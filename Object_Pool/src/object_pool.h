/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef OBJECT_POOL_H_
#define OBJECT_POOL_H_

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "kbase\memory\singleton.h"

using namespace std::placeholders;

template<typename T>
class ObjectFactory {
public:
    ObjectFactory(const ObjectFactory&) = delete;

    ObjectFactory(ObjectFactory&&) = delete;

    ObjectFactory& operator=(const ObjectFactory&) = delete;

    ObjectFactory& operator=(ObjectFactory&&) = delete;

    static ObjectFactory* GetInstance()
    {
        return kbase::Singleton<ObjectFactory>::instance();
    }

    std::shared_ptr<T> Get(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::weak_ptr<T>& object = pool_[key];
        std::shared_ptr<T> instance = object.lock();
        if (!instance) {
            auto deleter = std::bind(&ObjectFactory::DeleteObject, this, key, _1);
            instance.reset(new T(), deleter);
            object = instance;
        }

        return instance;
    }

private:
    ObjectFactory() = default;

    ~ObjectFactory() = default;

    void DeleteObject(const std::string& key, T* ptr)
    {
        Remove(key);
        delete ptr;
    }

    void Remove(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        pool_.erase(key);
    }

private:
    friend kbase::DefaultSingletonTraits<ObjectFactory>;
    std::mutex mutex_;
    std::map<std::string, std::weak_ptr<T>> pool_;
};

#endif  // OBJECT_POOL_H_