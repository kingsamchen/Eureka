//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef TESTS_STRINGIFICATION_H_
#define TESTS_STRINGIFICATION_H_

#include <deque>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "doctest/doctest.h"

namespace doctest {

template<typename T>
struct StringMaker<std::vector<T>> {
    static String convert(const std::vector<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& e : in) {
            oss << e << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename T>
struct StringMaker<std::list<T>> {
    static String convert(const std::list<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& e : in) {
            oss << e << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename T>
struct StringMaker<std::deque<T>> {
    static String convert(const std::deque<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& e : in) {
            oss << e << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename T>
struct StringMaker<std::set<T>> {
    static String convert(const std::set<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& e : in) {
            oss << e << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename T>
struct StringMaker<std::unordered_set<T>> {
    static String convert(const std::unordered_set<T>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& e : in) {
            oss << e << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename K, typename V>
struct StringMaker<std::map<K, V>> {
    static String convert(const std::map<K, V>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& [k, v] : in) {
            oss << k << "=" << v << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

template<typename K, typename V>
struct StringMaker<std::unordered_map<K, V>> {
    static String convert(const std::unordered_map<K, V>& in) {
        std::ostringstream oss;

        oss << "[";
        for (const auto& [k, v] : in) {
            oss << k << "=" << v << ", ";
        }
        oss << "]";

        return oss.str().c_str();
    }
};

} // namespace doctest

#endif // TESTS_STRINGIFICATION_H_
