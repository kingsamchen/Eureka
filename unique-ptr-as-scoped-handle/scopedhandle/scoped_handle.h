
#ifndef SCOPED_HANDLE_H_
#define SCOPED_HANDLE_H_

#include <cstdio>
#include <memory>

namespace scoped {

struct c_file_deleter {
    void operator()(FILE* ptr) const {
        fclose(ptr);
    }
};

using scoped_file = std::unique_ptr<FILE, c_file_deleter>;

} // namespace scoped

#endif // SCOPED_HANDLE_H_
