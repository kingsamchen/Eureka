/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <xutility>

template<typename BinaryFn>
struct InfixOp {
    explicit InfixOp(BinaryFn&& fn)
        : fn_(std::forward<BinaryFn>(fn))
    {}

    BinaryFn fn_;
};

template<typename BinaryFn>
InfixOp<BinaryFn> Infix(BinaryFn&& fn)
{
    return InfixOp<BinaryFn>(std::forward<BinaryFn>(fn));
}

template<typename T, typename BinaryFn>
struct InfixExpr {
    InfixExpr(T&& lhs, BinaryFn&& op)
        : lhs_(std::forward<T>(lhs)), op_(std::forward<BinaryFn>(op))
    {}

    template<typename U>
    decltype(auto) operator|(U&& rhs)
    {
        return op_(lhs_, std::forward<U>(rhs));
    }

    T lhs_;
    BinaryFn op_;
};

template<typename T, typename BinaryFn>
InfixExpr<T, BinaryFn> operator|(T&& lhs, InfixOp<BinaryFn> op)
{
    return InfixExpr<T, BinaryFn>(std::forward<T>(lhs), std::move(op.fn_));
}
