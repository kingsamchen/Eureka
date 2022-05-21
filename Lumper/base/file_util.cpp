//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "base/file_util.h"

#include <fstream>

namespace base {

void write_to_file(const std::filesystem::path& filepath, std::string_view data) {
    std::ofstream out(filepath);
    if (!out) {
        throw std::filesystem::filesystem_error(
                "cannot open file to write",
                filepath,
                std::error_code(errno, std::system_category()));
    }

    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!out) {
        throw std::filesystem::filesystem_error(
                "cannot write file",
                filepath,
                std::error_code(errno, std::system_category()));
    }
}

} // namespace base
