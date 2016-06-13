
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TEMPLATE_TYPE_CONSTRAINTS_TYPE_CONSTRAINTS_H_
#define TEMPLATE_TYPE_CONSTRAINTS_TYPE_CONSTRAINTS_H_

#include "basic_macros.h"

template<typename T>
class HasClone {
public:
    HasClone()
    {
        void (*pfn)() = &ValidateConstraints;
    }

    ~HasClone() = default;

    DISALLOW_COPY(HasClone);

    DISALLOW_MOVE(HasClone);

    static void ValidateConstraints()
    {
        T* (T::*fn)() const = &T::Clone;
        UNREFED_VAR(fn);
    }
};

template<typename D, typename B>
class IsDerivedFrom {
private:
    static std::false_type Check(...);
    static std::true_type Check(B*);

public:
    static constexpr const bool value = decltype(Check(static_cast<D*>(nullptr)))::value ;
};

#endif  // TEMPLATE_TYPE_CONSTRAINTS_TYPE_CONSTRAINTS_H_