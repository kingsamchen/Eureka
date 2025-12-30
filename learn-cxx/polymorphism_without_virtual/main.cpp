//
// Kingsley Chen
//

#include "fptr_in_base.hpp"

int main() {
    operation_a opa;
    operation_b opb;
    basic_operation* ptr = nullptr;
    ptr = &opa;
    ptr->perform();
    ptr = &opb;
    ptr->perform();
    return 0;
}
