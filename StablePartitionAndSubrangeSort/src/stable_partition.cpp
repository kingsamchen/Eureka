/*
 @ 0xCCCCCCCC
*/

#include <iterator>

template<typename BidirIt, typename UnaryPredicate>
BidirIt StablePartition(BidirIt first, BidirIt last, UnaryPredicate pred)
{
    if (first == last) {
        return first;
    }

    if (std::next(first) == last) {
        return pred(*first) ? last : first;
    }

    auto count = std::distance(first, last);
    BidirIt middle = std::next(first, count / 2);
    return std::rotate(StablePartition(first, middle, pred),
                       middle,
                       StablePartition(middle, last, pred));
}