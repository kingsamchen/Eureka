#ifndef LOWER_BOUND_INSERTER_H_
#define LOWER_BOUND_INSERTER_H_

#include <algorithm>
#include <functional>
#include <iterator>

namespace ext {

template<typename Container>
class lower_bound_insert_iterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    using Compare = std::function<bool(const typename Container::value_type& lhs,
                                       const typename Container::value_type& rhs)>;

    lower_bound_insert_iterator() noexcept
        : container_(nullptr)
    {}

    explicit lower_bound_insert_iterator(Container& c, Compare cmp) noexcept
        : container_(std::addressof(c)),
          cmp_(std::move(cmp))
    {}

    lower_bound_insert_iterator& operator=(const typename Container::value_type& value)
    {
        auto it = std::lower_bound(std::begin(*container_), std::end(*container_), value, cmp_);
        container_->insert(it, value);
        return *this;
    }

    lower_bound_insert_iterator& operator=(typename Container::value_type&& value)
    {
        auto it = std::lower_bound(std::begin(*container_), std::end(*container_), value, cmp_);
        container_->insert(it, std::move(value));
        return *this;
    }

    lower_bound_insert_iterator& operator*() noexcept
    {
        return *this;
    }

    lower_bound_insert_iterator& operator++() noexcept
    {
        return *this;
    }

    lower_bound_insert_iterator operator++(int)
    {
        return *this;
    }

private:
    Container* container_;
    Compare cmp_;
};

template<typename Container, typename Cmp = std::less<>>
[[nodiscard]] auto lower_bound_inserter(Container& c, Cmp&& cmp = Cmp{})
{
    return lower_bound_insert_iterator<Container>(c, std::forward<Cmp>(cmp));
}

}   // namespace ext

#endif  // LOWER_BOUND_INSERTER_H_
