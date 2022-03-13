
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
