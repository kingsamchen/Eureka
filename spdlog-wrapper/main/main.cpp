//
// Kingsley Chen
//

#include "logging.hpp"

#include "gflags/gflags.h"

DEFINE_int32(repeat_cnt, 1, "");

namespace {

auto with_reqid(const std::string& reqid) {
    //return base::logger::field(std::string{"reqid"}, reqid);
    return base::logger::field("reqid", reqid);
}

} // namespace

int main(int argc, char* argv[]) {
    (void)argc;
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    base::logger::init_logging(argv[0]);
    base::logger::get_pub().info(
            base::logger::with_fields(
                    base::logger::field(std::string{"type"}, std::string{"string"}),
                    with_reqid("0xdeadbeef")),
            std::string{"hello {}"}, 42);

    return 0;
}
