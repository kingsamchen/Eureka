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

#define GUARANTOR_A(x) GUARANTOR_OP(x, B)
#define GUARANTOR_B(x) GUARANTOR_OP(x, A)
#define GUARANTOR_OP(x, next) \
    GUARANTOR_A.current_value(#x, (x)).GUARANTOR_ ## next

#define MAKE_GUARANTOR(exp) \
    Guarantor(exp, __FILE__, __LINE__)

// TODO: add ENSURE macro.

class Guarantor {
public:
    Guarantor(const char* msg, const char* file_name, int line)
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

    // access stubs
    Guarantor& GUARANTOR_A = *this;
    Guarantor& GUARANTOR_B = *this;

private:
    std::ostringstream exception_desc_;
};

#endif  // YET_ANOTHER_ENSURE_ENSURE_H_