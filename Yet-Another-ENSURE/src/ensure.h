/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef YET_ANOTHER_ENSURE_ENSURE_H_
#define YET_ANOTHER_ENSURE_ENSURE_H_

#include <sstream>

#include "basic_macros.h"

enum class Action : int {
    CHECK,
    RAISE,
    RAISE_WITH_DUMP
};

#if defined(NDEBUG)
#define ACTION_IS_ON(action) (Action::##action != Action::CHECK)
#else
#define ACTION_IS_ON(action) true
#endif

#define GUARANTOR_A(x) GUARANTOR_OP(x, B)
#define GUARANTOR_B(x) GUARANTOR_OP(x, A)
#define GUARANTOR_OP(x, next) \
    GUARANTOR_A.current_value(#x, (x)).GUARANTOR_ ## next

#define MAKE_GUARANTOR(cond, action) \
    Guarantor(cond, __FILE__, __LINE__, Action::##action)

#define ENSURE(action, cond) \
    !(ACTION_IS_ON(action) && !(cond)) ? (void)0 : MAKE_GUARANTOR(#cond, action).GUARANTOR_A

class Guarantor {
public:
    Guarantor(const char* msg, const char* file_name, int line, Action action)
        : action_required_(action)
    {
        exception_desc_ << "Failed: " << msg << "\nFile: " << file_name << " Line: " << line
                        << "\nCurrent Variables:\n";
    }

    ~Guarantor() = default;

    DISALLOW_COPY(Guarantor);

    DISALLOW_MOVE(Guarantor);

    // Incorporates variable value.
    template<typename T>
    Guarantor& current_value(const char* name, const T& value)
    {
        exception_desc_ << "    " << name << " = " << value << "\n";
        return *this;
    }

    void Require() const;

    // access stubs
    Guarantor& GUARANTOR_A = *this;
    Guarantor& GUARANTOR_B = *this;

private:
    void Check() const;

    void Raise() const;

    void RaiseWithDump() const;

private:
    Action action_required_;
    std::ostringstream exception_desc_;
};

#endif  // YET_ANOTHER_ENSURE_ENSURE_H_