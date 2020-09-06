#include <iostream>

#include "tagged_bool.h"

int main()
{
    using WithAdvancedMode = alias::TaggedBool<struct AdvancedModeTag>;
    using WithMultithreading = alias::TaggedBool<struct MultithreadingTag>;

    constexpr WithAdvancedMode mode_ok = true;
    constexpr WithMultithreading mt(mode_ok);
    constexpr WithMultithreading sta = false;

    return 0;
}
