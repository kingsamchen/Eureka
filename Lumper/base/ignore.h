//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef BASE_IGNORE_H_
#define BASE_IGNORE_H_

namespace base {

template<typename T>
void ignore_unused(T&&) {}

} // namespace base

#define FORCE_AS_MEMBER_FUNCTION() \
    ::base::ignore_unused(this)

#endif // BASE_IGNORE_H_
