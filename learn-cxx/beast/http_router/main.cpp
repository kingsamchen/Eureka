// Kingsley Chen

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, const char* argv[]) { // NOLINT(bugprone-exception-escape)
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
