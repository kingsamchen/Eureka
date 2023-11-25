//
// Kingsley Chen
//

#pragma once

#include <ranges>

// Use ranges's view concept as template arguments
// Each view class must inherit from `view_interface`, which provides quite a few methods, and
// the class must also offer `begin()` and `end()`.

template<std::ranges::view V1, std::ranges::view V2>
    requires std::same_as<std::ranges::range_value_t<V1>, std::ranges::range_value_t<V2>> &&
             std::same_as<std::ranges::range_reference_t<V1>, std::ranges::range_reference_t<V2>>
class chain_view : public std::ranges::view_interface<chain_view<V1, V2>> {

    struct sentinel {
        std::ranges::sentinel_t<V2> s;
    };

    struct iterator {
        using iterator_category =
                std::conditional_t<std::ranges::forward_range<V1> && std::ranges::forward_range<V2>,
                                   std::forward_iterator_tag, std::input_iterator_tag>;
        using difference_type = std::common_type_t<std::ranges::range_difference_t<V1>,
                                                   std::ranges::range_difference_t<V2>>;
        // Requires traits<V1> == traits<V2>
        using value_type = std::ranges::range_value_t<V1>;
        using reference = std::ranges::range_reference_t<V1>;
        using pointer = value_type*;

        iterator() = default;

        iterator(chain_view& parent)
            : parent_(&parent),
              iter1_(std::ranges::begin(parent.v1_)),
              iter2_(std::ranges::begin(parent.v2_)) {}

        reference operator*() const {
            return first_part_done_ ? *iter2_ : *iter1_;
        }

        iterator& operator++() {
            if (first_part_done_) {
                ++iter2_;
            } else {
                if (std::ranges::end(parent_->v1_) == ++iter1_) {
                    first_part_done_ = true;
                }
            }
            return *this;
        }

        iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const sentinel& st) const {
            return first_part_done_ && iter2_ == st.s;
        }

        bool operator==(const iterator&) const = default;

        chain_view* parent_{nullptr};
        std::ranges::iterator_t<V1> iter1_;
        std::ranges::iterator_t<V2> iter2_;
        bool first_part_done_{false};
    };

public:
    chain_view() = default;

    chain_view(V1 v1, V2 v2)
        : v1_(v1),
          v2_(v2) {}

    auto begin() {
        return iterator{*this};
    }

    auto end() {
        return sentinel{std::ranges::end(v2_)};
    }

private:
    V1 v1_;
    V2 v2_;
};
