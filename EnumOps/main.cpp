/*
 @ 0xCCCCCCCC
*/

#include <cassert>
#include <string>

#include "enum_ops.h"

enum ChangeLevel : unsigned int {
    None = 0,
    Local = 1 << 0,
    Server = 1 << 1,
    All = Local | Server
};

int main()
{
    using namespace enum_ops;

    ChangeLevel level = ChangeLevel::None;
    level |= ChangeLevel::Local;
    level |= ChangeLevel::Server;

    assert(level == ChangeLevel::All);
    assert(!!(level & ChangeLevel::Local));
    assert(!!(level & ChangeLevel::Server));

    return 0;
}
