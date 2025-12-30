//
// Kingsley Chen
//

#pragma once

#include <fmt/format.h>

struct basic_operation {
    using fptr_type = void (*)(basic_operation*);

    explicit basic_operation(fptr_type fptr)
        : fptr_(fptr) {}

    void perform() {
        fptr_(this);
    }

    fptr_type fptr_{nullptr};
};

class operation_a : public basic_operation {
public:
    operation_a()
        : basic_operation(&operation_a::do_a) {}

    static void do_a(basic_operation* op) {
        static_cast<operation_a*>(op)->do_a_impl(); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }

    void do_a_impl() {
        fmt::println("perform operatio a");
    }
};

class operation_b : public basic_operation {
public:
    operation_b()
        : basic_operation(&operation_b::do_b) {}

    static void do_b(basic_operation* op) {
        static_cast<operation_b*>(op)->do_b_impl(); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    }

    void do_b_impl() {
        fmt::println("perform operatio b");
    }
};
