//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include "base/subprocess.h"

TEST_CASE("Demo") {
    base::subprocess proc({"/bin/ls", "/home/kingsleychen"});
    proc.wait();
}
