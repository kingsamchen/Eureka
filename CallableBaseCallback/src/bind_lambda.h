/*
 @ 0xCCCCCCCC
*/

#ifndef BASE_EXT_BIND_LAMBDA_H_
#define BASE_EXT_BIND_LAMBDA_H_

#include <functional>
#include <utility>

#include "base/bind.h"

namespace base {

template<typename R>
auto InvokeLambda(const std::function<R()>& fn) -> R
{
    return fn();
}

template<typename F>
auto BindLambda(F&& lambda) -> base::Callback<typename std::result_of<F()>::type()>
{
    using return_type = typename std::result_of<F()>::type;

    return base::Bind(InvokeLambda<return_type>, std::function<return_type()>(std::forward<F>(lambda)));
}

}   // namespace base

#endif  // BASE_EXT_BIND_LAMBDA_H_
